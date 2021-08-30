#include <memory>
#include <sstream>
#include "spdlog/fmt/fmt.h"

#include "share/exception.h"
#include "share/stack_trace.h"

namespace jm
{

Exception::Exception(const std::string &message) : message(message)
{
    stack_trace = StackTrace::get_stack_trace(1);
}

const char *Exception::what() const noexcept
{
    // Note, buffer is mutable
    buffer = message;
    return buffer.c_str();
}

std::string Exception::get_message_and_stack_trace() const
{
    std::string text = message;
    for (int i = 0; i < stack_trace.size(); i++) {
        text += fmt::format("\n   {:02d} at {}", i, stack_trace[i]);
    }

    return text;
}

}