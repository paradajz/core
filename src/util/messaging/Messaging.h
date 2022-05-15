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

#include <inttypes.h>
#include <stddef.h>
#include <functional>
#include <memory>
#include <vector>
#include "core/src/util/RingBuffer.h"

// Publish-subscribe and service call mechanism inspired by ROS (Robot Operating System).
// All messages and services need to be defined through YAML file with .msg extension.
// Code is generated from those files for simpler usage. An example file is given in this directory.
// Below is a generated code from the example message:

// Header (ExampleMessage.msg.h):

/*

#pragma once

#include "core/src/util/messaging/Messaging.h"

#include "core/src/util/Util.h"

namespace Messages
{
    class SensorStateChangeNotification : public core::util::messaging::Notification
    {
        public:
        SensorStateChangeNotification() = default;

        static constexpr bool USE_QUEUE  = true;
        static constexpr bool QUEUE_SIZE = 16;

        struct data_t
        {
            size_t index;
            bool   state;
        };

        data_t data;

        private:
        friend class core::util::messaging::Server;
        static std::weak_ptr<core::util::messaging::Publisher<SensorStateChangeNotification>>               _publisher;
        static std::vector<std::weak_ptr<core::util::messaging::Subscriber<SensorStateChangeNotification>>> _subscribers;
    };

    class SensorStateChangeService : public core::util::messaging::Service
    {
        public:
        SensorStateChangeService() = default;

        struct request_t
        {
            size_t index;
        };

        request_t request;

        struct response_t
        {
            bool state;
        };

        response_t response;

        private:
        friend class core::util::messaging::Server;
        static std::function<bool(SensorStateService&)> _serviceCallback;
    };

    class SensorStateChangeService : public core::util::messaging::Service
    {
        public:
        SensorStateChangeService() = default;

        struct response_t
        {
            bool state;
        };

        response_t response;

        private:
        friend class core::util::messaging::Server;
        static std::function<bool(SpecificSensorStateService&)> _serviceCallback;
    };
}    // namespace Messages

*/

// Source (ExampleMessage.msg.cpp):

/*

#include "ExampleMessage.msg.h"

using namespace Messages;

std::vector<std::weak_ptr<core::util::messaging::Subscriber<SensorStateChangeNotification>>> SensorStateChangeNotification::_subscribers;
std::weak_ptr<core::util::messaging::Publisher<SensorStateChangeNotification>>               SensorStateChangeNotification::_publisher;
std::function<bool(SensorStateService&)>                                                     SensorStateService::_serviceCallback;
std::function<bool(SpecificSensorStateService&)>                                             SpecificSensorStateService::_serviceCallback;

*/

namespace core::util::messaging
{
    // Base class for all notifications.
    // Notification is a broadcasting message anyone can listen to.
    class Notification
    {
        public:
        Notification() = default;
    };

    // Base class for all services.
    // Service is a request-response type of message.
    class Service
    {
        public:
        Service() = default;
    };

    // Forward declarations

    class Server;

    template<class T>
    class Subscriber;

    template<class T>
    class Publisher;

    // Used only for subscribers to notifications.
    // Taken into account only if subscriber uses queue:
    // if queue is used and pushed notification returns DEFER, notification
    // will not be removed from queue until the call returns DONE.
    // For subscribers without queue, return value is ignored and notification
    // is removed from queue after the call.
    enum class notificationResult_t : uint8_t
    {
        DONE,
        DEFER,
    };

    template<class NotificationType>
    class Subscriber
    {
        static_assert(std::is_base_of<Notification, NotificationType>::value, "Type not a notification");

        public:
        using callback_t = std::function<notificationResult_t(const NotificationType&)>;

        class Queue
        {
            public:
            virtual ~Queue()                                            = default;
            virtual size_t size()                                       = 0;
            virtual bool   peek(NotificationType& notification)         = 0;
            virtual bool   remove(NotificationType& notification)       = 0;
            virtual bool   insert(const NotificationType& notification) = 0;
        };

        Subscriber(callback_t handler, std::shared_ptr<Queue> queue)
            : _handler(std::move(handler))
            , _queue(std::move(queue))
        {}

        Subscriber(callback_t handler)
            : _handler(std::move(handler))
        {}

        // Checks if there are any notifications in queue: if queue
        // is disabled this won't do anything.
        void flush()
        {
            const auto SIZE = _queue->size();

            for (size_t i = 0; i < SIZE; i++)
            {
                NotificationType notification;

                if (_queue->peek(notification))
                {
                    if (_handler(notification) == notificationResult_t::DONE)
                    {
                        _queue->remove(notification);
                    }

                    // defer otherwise - leave in buffer until DONE is returned
                }
                else
                {
                    // nothing else in queue
                    break;
                }
            }
        }

        private:
        // Call a subscriber through previously provided handler with specified Notification object.
        void notify(const NotificationType& message)
        {
            if (_queue != nullptr)
            {
                _queue->insert(message);
            }
            else
            {
                // ignore return value
                _handler(message);
            }
        }

        friend class Server;
        friend class Publisher<NotificationType>;

        callback_t             _handler;
        std::shared_ptr<Queue> _queue;
    };

    template<class NotificationType>
    class Publisher
    {
        static_assert(std::is_base_of<Notification, NotificationType>::value, "Type not a notification");

        // Publisher keeps a list of all subscribers.
        using subscriberList_t = std::vector<std::weak_ptr<Subscriber<NotificationType>>>;

        public:
        Publisher(subscriberList_t& subscribers)
            : _subscribers(subscribers)
        {}

        // Push Notification object to anyone who listens.
        void publish(const NotificationType& data)
        {
            for (size_t i = 0; i < _subscribers.size(); i++)
            {
                auto sub = _subscribers[i].lock();

                if (sub)
                {
                    sub->notify(data);
                }
                else
                {
                    // subscriber doesn't exist anymore
                    _subscribers.erase(_subscribers.begin() + i);
                    i--;
                }
            }
        }

        private:
        friend class Server;

        void updateSubscribers(const subscriberList_t& subscribers)
        {
            _subscribers = subscribers;
        }

        subscriberList_t _subscribers;
    };

    class Server
    {
        public:
        Server() = default;

        // helper if used as singleton
        static Server& instance()
        {
            static Server instance;
            return instance;
        }

        // Registers a publisher by returning a pointer to Publisher class.
        template<class NotificationType>
        std::shared_ptr<Publisher<NotificationType>> advertise()
        {
            static_assert(std::is_base_of<Notification, NotificationType>::value, "Type not a notification");

            auto pub                     = std::make_shared<Publisher<NotificationType>>(NotificationType::_subscribers);
            NotificationType::_publisher = pub;

            return pub;
        }

        // When registering service, a callback needs to be provided
        // which will be called every time callService is called by user.
        template<class ServiceType>
        void advertiseService(std::function<bool(ServiceType&)> handler)
        {
            static_assert(std::is_base_of<Service, ServiceType>::value, "Type not a service");

            ServiceType::_serviceCallback = std::move(handler);
        }

        // Calls a service for a given type through template argument.
        // Service object is passed to service advertiser - depending on the callback,
        // the data is usually changed there.
        // Returns false if service provider for a given type doesn't exist.
        // Othwise, a return value from registered callback is returned.
        template<class ServiceType>
        bool callService(ServiceType& service)
        {
            static_assert(std::is_base_of<Service, ServiceType>::value, "Type not a service");

            if (ServiceType::_serviceCallback == nullptr)
            {
                return false;
            }

            return ServiceType::_serviceCallback(service);
        }

        // Subscribes to a notification of a given type through template argument via handler given through function argument.
        template<class NotificationType>
        std::shared_ptr<Subscriber<NotificationType>> subscribe(typename Subscriber<NotificationType>::callback_t handler)
        {
            static_assert(std::is_base_of<Notification, NotificationType>::value, "Type not a notification");
            std::shared_ptr<Subscriber<NotificationType>> sub;

            if constexpr (NotificationType::USE_QUEUE == true)
            {
                auto queue = std::make_shared<Queue<NotificationType>>();
                sub        = std::make_shared<Subscriber<NotificationType>>(handler, queue);
            }

            if constexpr (NotificationType::USE_QUEUE == false)
            {
                sub = std::make_shared<Subscriber<NotificationType>>(handler);
            }

            NotificationType::_subscribers.push_back(sub);
            auto pub = NotificationType::_publisher.lock();

            if (pub)
            {
                pub->updateSubscribers(NotificationType::_subscribers);
                return sub;
            }

            // nobody is publishing this notification
            return nullptr;
        }

        private:
        // Wrapper around RingBuffer - this makes is simpler to inject into Notification object
        // since Ring buffer requires template arguments.
        template<class NotificationType>
        class Queue : public Subscriber<NotificationType>::Queue
        {
            public:
            Queue() = default;

            size_t size() override
            {
                return _queue.size();
            }

            bool peek(NotificationType& message) override
            {
                return _queue.peek(message);
            }

            bool remove(NotificationType& message) override
            {
                return _queue.remove(message);
            }

            bool insert(const NotificationType& message) override
            {
                return _queue.insert(message);
            }

            private:
            core::util::RingBuffer<NotificationType, NotificationType::QUEUE_SIZE> _queue;
        };
    };
}    // namespace core::util::messaging

// Helper macro for use with singleton Server instance for easier typing
#define MessagingServer core::util::messaging::Server::instance()
