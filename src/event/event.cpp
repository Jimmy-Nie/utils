#include "event.h"
#include "exception/exception.h"
#include <utility>

namespace jm
{
Event::Event(std::string  name, std::vector<Value> arg)
    : name(std::move(name))
    , arguments(std::move(arg))
{
}

unsigned int Event::get_argument_count() const
{
    return arguments.size();
}

Value Event::get_argument(unsigned int index) const
{
    if (index >= arguments.size())
        throw Exception("Arguments out of bounds for game event " + name);
    return arguments[index];
}

void Event::add_argument(const Value &value)
{
    arguments.push_back(value);
}

std::string Event::to_string() const
{
    std::string event_info = name + "(";

    for (unsigned int i = 0; i < arguments.size(); ++i)
    {
        if (i > 0)
            event_info += ",";

        event_info += Value::to_string(arguments[i]);
    }

    event_info += ")";

    return event_info;
}

}
