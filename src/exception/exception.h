#pragma once

#include <vector>
#include <stdexcept>
#include <string>

namespace jm
{
/// \brief Top-level exception class.
class Exception : public std::exception {
public:
    /// \brief Constructs an exception object.
    explicit Exception(const std::string &message);

    /// \brief Destructs an exception object
    virtual ~Exception() noexcept {};

    /// \brief Description of exception.
    std::string message;

    /// \brief Returns description of exception
    virtual const char *what() const noexcept override;

    /// \brief Returns the message and call stack present when the exception object was created, formatted using newlines.
    std::string get_message_and_stack_trace() const;

private:
    std::vector<std::string> stack_trace;
    mutable std::string buffer;
};
}