#pragma once

#include <map>
#include "event.h"

namespace jm
{

/// \brief: bind the event with function, 
/// if the event happened, exec the binding function     
template<typename EventType, typename FuncType>
class EventDispatcher;

template<typename EventType, typename ReturnType, typename ...Args>
class EventDispatcher<EventType, ReturnType(Args...)>
{
public:
    using Callback = std::function<ReturnType(Args...)>;

    void appendListener(const EventType &event, const Callback &cb)
    {
        event_handlers[event] = cb;
    }

    /// \brief Dispatches the event object.
    ///
    /// \return true if the event handler is invoked and false if the event handler is not found.
    bool dispatch(const EventType &event, Args... args)
    {
        auto it = event_handlers.find(event);
        if (it != event_handlers.end() && (bool)it->second)
        {
            it->second(args...);
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    std::map<EventType, Callback> event_handlers;
};
}
