
/*
    Copyright 2017-2022 Igor Petrovic

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <functional>

namespace core::util
{
    class Task
    {
        public:
        using taskFunc_t = std::function<void()>;

        enum class taskType_t : uint8_t
        {
            ONE_SHOT,
            RECURRING
        };

        Task(size_t taskID, uint32_t timeout, taskFunc_t&& func)
            : TASK_ID(taskID)
            , TIMEOUT(timeout)
            , _func(std::move(func))
        {}

        Task(size_t taskID, uint32_t timeout, taskFunc_t& func)
            : TASK_ID(taskID)
            , TIMEOUT(timeout)
            , _func(func)
        {}

        void start()
        {
            _timeRemaining = TIMEOUT;
        }

        void stop()
        {
            _timeRemaining = 0;
        }

        bool isRunning()
        {
            return _timeRemaining;
        }

        private:
        friend class Scheduler;

        const size_t   TASK_ID;
        const uint32_t TIMEOUT;
        int32_t        _timeRemaining = 0;
        taskFunc_t     _func;
    };

    class Scheduler
    {
        public:
        Scheduler() = default;

        std::shared_ptr<Task> registerTask(uint32_t timeout, Task::taskType_t taskType, Task::taskFunc_t func)
        {
            auto task = std::make_shared<Task>(_taskCounter++, timeout, std::move(func));
            _tasks.push_back({ task, taskType });

            return task;
        }

        protected:
        virtual bool init()
        {
            return true;
        }

        void update(uint32_t elapsedMs)
        {
            for (size_t i = 0; i < _tasks.size(); i++)
            {
                auto task = _tasks[i].task.lock();

                if (task->_timeRemaining)
                {
                    task->_timeRemaining -= elapsedMs;

                    if (task->_timeRemaining <= 0)
                    {
                        if (task->_func != nullptr)
                        {
                            task->_func();
                        }

                        if (_tasks[i].type == Task::taskType_t::ONE_SHOT)
                        {
                            task->_timeRemaining = 0;
                        }
                        else
                        {
                            task->start();
                        }
                    }
                }
            }
        }

        void clear()
        {
            _tasks = {};
        }

        private:
        struct taskInternal_t
        {
            std::weak_ptr<Task> task;
            Task::taskType_t    type;
        };

        std::vector<taskInternal_t> _tasks;
        size_t                      _taskCounter = 0;
    };
}    // namespace core::util