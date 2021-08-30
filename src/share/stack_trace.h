//
// Created by binfeng.yang on 2019/7/11.
//
#pragma once

#include <vector>
#include <cstdint>
#include <string>
namespace jm
{
/// \brief StackTrace class.
class StackTrace {
public:
    ///\brief Return the call stack addr
    /** <p>On Linux, to obtain function names, remember to link with the -rdynamic flag, if HAVE_EXECINFO_H</p>*/
    static std::vector<std::string> get_stack_trace(int frames_skip = 0, void * secret = nullptr);

private:
    enum {
        max_frames = 32
    };
};
}