
#include "share/stack_trace.h"
#include "spdlog/fmt/fmt.h"

#ifndef WIN32

#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#if defined(HAVE_SYS_SYSCTL_H) && \
        !defined(_SC_NPROCESSORS_ONLN) && !defined(_SC_NPROC_ONLN)
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

#if defined HAVE_EXECINFO_H
#include <execinfo.h>
#else
#include <dlfcn.h>
#include <unwind.h>
#include <ucontext.h>
#endif
#include <cxxabi.h>
#include <cstring>
#include <cstdlib>
#endif
#ifdef _MSC_VER
#ifdef DEBUG
#include <crtdbg.h>
#endif
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
#endif
#ifdef __MINGW32__
#include <dbghelp.h>
#include <malloc.h>
#endif
#include <mutex>
namespace jm
{
#if !defined(WIN32)
std::string get_function_name(const std::string &function)
{
    std::string ret = function;
    int status;
    char *new_function = abi::__cxa_demangle(function.c_str(), nullptr, nullptr, &status);
    if (new_function) {   // Was correctly decoded
        ret = new_function;
    }
    free(new_function);
    return ret;
}
#endif

#if !defined HAVE_EXECINFO_H && !defined WIN32
struct trace_unwind_state_t {
    size_t frames_to_skip;
    _Unwind_Word *current;
    _Unwind_Word *end;
};

static _Unwind_Reason_Code trace_unwind_callback(::_Unwind_Context *context, void *arg)
{
    trace_unwind_state_t *const state = reinterpret_cast<trace_unwind_state_t *>(arg);
    if (state->frames_to_skip) {
        --state->frames_to_skip;
        return _Unwind_GetIP(context) ? ::_URC_NO_REASON : ::_URC_END_OF_STACK;
    }
    _Unwind_Ptr ip = _Unwind_GetIP(context);
    if (ip) {
        *state->current = ip;
        ++state->current;
        if (state->current == state->end) {
            return ::_URC_END_OF_STACK;
        }
    }
    return ::_URC_NO_REASON;
}
#endif

std::vector<std::string> StackTrace::get_stack_trace(int frames_skip, void *secret)
{
#ifdef WIN32
	void *frames[max_frames];
	unsigned short num_frames = 0;

	// RtlCaptureStackBackTrace is only available on Windows XP or newer versions of Windows
	typedef WORD(NTAPI FuncRtlCaptureStackBackTrace)(DWORD, DWORD, PVOID *, PDWORD);
	HMODULE module = LoadLibrary(TEXT("kernel32.dll"));
    int *out_hash = nullptr;
	if (module)
	{
		FuncRtlCaptureStackBackTrace *ptrRtlCaptureStackBackTrace = (FuncRtlCaptureStackBackTrace *)GetProcAddress(module, "RtlCaptureStackBackTrace");
		if (ptrRtlCaptureStackBackTrace)
			num_frames = ptrRtlCaptureStackBackTrace(frames_skip + 1, max_frames, frames, (DWORD *)out_hash);
		FreeLibrary(module);
	}

	if (num_frames == 0 && out_hash)
		*out_hash = 0;

	static std::recursive_mutex mutex;
	std::unique_lock<std::recursive_mutex> mutex_lock(mutex);

	BOOL result = SymInitialize(GetCurrentProcess(), NULL, TRUE);
	if (!result)
		return std::vector<std::string>();

	std::vector<std::string> backtrace_text;
	for (unsigned short i = 0; i < num_frames; i++)
	{
		unsigned char buffer[sizeof(IMAGEHLP_SYMBOL64) + 128];
		IMAGEHLP_SYMBOL64 *symbol64 = reinterpret_cast<IMAGEHLP_SYMBOL64*>(buffer);
		memset(symbol64, 0, sizeof(IMAGEHLP_SYMBOL64) + 128);
		symbol64->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		symbol64->MaxNameLength = 128;

		DWORD64 displacement = 0;
		BOOL result = SymGetSymFromAddr64(GetCurrentProcess(), (DWORD64)frames[i], &displacement, symbol64);
		if (result)
		{
			IMAGEHLP_LINE64 line64;
			DWORD displacement = 0;
			memset(&line64, 0, sizeof(IMAGEHLP_LINE64));
			line64.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			result = SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)frames[i], &displacement, &line64);
			if (result)
			{
				backtrace_text.push_back(
					fmt::format("{} ({}, line {})",
						symbol64->Name,
						line64.FileName,
						(int)line64.LineNumber));
			}
			else
			{
				backtrace_text.push_back(symbol64->Name);
			}
		}
	}

	SymCleanup(GetCurrentProcess());
	return backtrace_text;

#elif !defined(HAVE_EXECINFO_H)
    _Unwind_Word stacks[max_frames];
    size_t num_frames = 0;
#ifndef __GLIBC__
    if (secret != nullptr) {
        auto *uc  = (ucontext_t *)secret;
        auto * fp = reinterpret_cast<unsigned long*>(uc->uc_mcontext.arm_fp);
        stacks[num_frames++] = uc->uc_mcontext.arm_pc;
        while (num_frames < max_frames && fp != nullptr) {
            auto * pc = (unsigned long *)(*fp);
            stacks[num_frames++] = (_Unwind_Word)pc;
            auto next_fp = (unsigned long *)(*(fp - 1));
            if (next_fp < fp) {
                break;
            }
            fp = next_fp;
        }
    } else
#endif
    {
        trace_unwind_state_t state{
            .frames_to_skip = (size_t)(frames_skip),
            .current = stacks,
            .end = stacks + max_frames
        };
        ::_Unwind_Backtrace(&trace_unwind_callback, &state);
        num_frames = state.current - &stacks[0];
    }
    std::vector<std::string> ret;
    for (size_t i = 0; i < num_frames; ++i) {
        Dl_info info;
        int rc = dladdr((void*)stacks[i], &info);
        if (!rc) {
            continue;
        }
        std::string function;
        if (info.dli_sname) {
            function = get_function_name(info.dli_sname);
        }

        ret.push_back(fmt::format("{}: ({}) [0x{:x}]", info.dli_fname, function, stacks[i]));
    }
    return ret;
#else
    void *frames[max_frames];
    std::memset(frames, 0, (sizeof(void *)) * max_frames);
    int num_frames = backtrace(frames, max_frames);

    char **strings;
    strings = backtrace_symbols(frames, num_frames);
    if (!strings) {
        return std::vector<std::string>();
    }

    std::vector<std::string> backtrace_text;

    for (int cnt = 0; cnt < num_frames; cnt++) {
        // Decode the strings
        char *ptr = strings[cnt];
        char *filename = ptr;
        const char *function = "";

        // Find function name
        while (*ptr) {
            if (*ptr == '(')    // Found function name
            {
                *(ptr++) = 0;
                function = ptr;
                break;
            }
            ptr++;
        }

        // Find offset
        if (function[0]) {   // Only if function was found
            while (*ptr) {
                if (*ptr == '+') {   // Found function offset
                    *(ptr++) = 0;
                    break;
                }
                if (*ptr == ')') {   // Not found function offset, but found, end of function
                    *(ptr++) = 0;
                    break;
                }
                ptr++;
            }
        }

        auto func_nume = get_function_name(function);
        backtrace_text.push_back(fmt::format("{} ({})", filename, func_nume));
    }

    free(strings);
    return backtrace_text;
#endif

}
}