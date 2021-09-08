#pragma once

#include <string>
#include <vector>
#include "value.h"

namespace jm
{

/// \brief Event
class Event
{
public:
    /// Constructs a Event
    Event(std::string name, std::vector<Value> arg = {});

    /// \return The name of this event.
    std::string get_name() const { return name; };

    /// \return The number of arguments stored in this event.
    unsigned int get_argument_count() const;

    /// Retrieves an argument in this event.
    /// \param index Index number of the argument to retrieve.
    /// \return A Value object containing the argument value.
    Value get_argument(unsigned int index) const;

    /// Adds an argument into this event.
    /// \param value The argument to store inside this event.
    void add_argument(const Value &value);

    /// \return A string representation of this event, including all of its arguments.
    std::string to_string() const;

private:
    std::string name;
    std::vector<Value> arguments;
};

}

