
/*
    Copyright 2017-2023 Igor Petrovic

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

#include <inttypes.h>
#include <cstdarg>
#include <stdio.h>
#include <functional>
#include "core/MCU.h"

namespace core::util
{
    template<size_t bufferSize>
    class Logger
    {
        public:
        enum class logLevel_t : uint8_t
        {
            INFO,
            WARNING,
            ERROR,
            AMOUNT
        };

        using writer_t = std::function<bool(const char*)>;

        Logger(writer_t&& writer)
            : _writer(std::move(writer))
        {}

        bool write(logLevel_t level, const char* file, int line, const char* message, ...)
        {
            if (level != logLevel_t::AMOUNT)
            {
                if (!_writer(LOG_LEVEL_STRING[static_cast<uint8_t>(level)]))
                {
                    return false;
                }
            }

            snprintf(_logBuffer, sizeof(_logBuffer), "[%s:%d] ", file, line);

            if (!_writer(_logBuffer))
            {
                return false;
            }

            snprintf(_logBuffer, sizeof(_logBuffer), "[%lu] ", core::mcu::timing::ms());

            if (!_writer(_logBuffer))
            {
                return false;
            }

            va_list args;
            va_start(args, message);
            vsnprintf(_logBuffer, sizeof(_logBuffer), message, args);
            va_end(args);

            if (!_writer(_logBuffer))
            {
                return false;
            }

            if (!_writer("\r\n"))
            {
                return false;
            }

            return true;
        }

        private:
        static constexpr size_t LOG_BUFFER_SIZE = bufferSize;
        writer_t                _writer         = nullptr;
        char                    _logBuffer[LOG_BUFFER_SIZE];

        static constexpr const char* LOG_LEVEL_STRING[static_cast<uint8_t>(logLevel_t::AMOUNT)] = {
            "[INFO] ",
            "[WARNING] ",
            "[ERROR] ",
        };
    };
}    // namespace core::util

// __FILE_NAME__ is clang specific
#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

#define CORE_LOGGER_DECLARE(loggerName, bufferSize)                 \
    using CoreLogger_##loggerName = core::util::Logger<bufferSize>; \
    extern CoreLogger_##loggerName coreLoggerInstance_##loggerName;

#define CORE_LOGGER_CREATE(loggerName, writerFunc) \
    CoreLogger_##loggerName coreLoggerInstance_##loggerName = CoreLogger_##loggerName(writerFunc);

#define CORE_LOG_INFO(loggerName, ...)  (coreLoggerInstance_##loggerName.write(CoreLogger_##loggerName::logLevel_t::INFO, __FILE_NAME__, __LINE__, __VA_ARGS__))
#define CORE_LOG_WARN(loggerName, ...)  (coreLoggerInstance_##loggerName.write(CoreLogger_##loggerName::logLevel_t::WARNING, __FILE_NAME__, __LINE__, __VA_ARGS__))
#define CORE_LOG_ERROR(loggerName, ...) (coreLoggerInstance_##loggerName.write(CoreLogger_##loggerName::logLevel_t::ERROR, __FILE_NAME__, __LINE__, __VA_ARGS__))