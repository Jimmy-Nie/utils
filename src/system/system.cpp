
#include "system.h"
#include "spdlog/fmt/fmt.h"
#include <thread>
#include <share/exception.h>

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>

#if defined(HAVE_SYS_SYSCTL_H) && \
    !defined(_SC_NPROCESSORS_ONLN) && !defined(_SC_NPROC_ONLN)
#include <sys/param.h>
#include <sys/sysctl.h>
#endif
#include <cxxabi.h>
#include <cstring>
#include <cstdlib>
#include <libgen.h>
#include <sys/stat.h>
#include <climits>

#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __MINGW32__
#include <dbghelp.h>
#include <malloc.h>
#endif

#ifdef __ANDROID__
// Remove this, when it works without
extern "C" {
	// Note, this still does not work. humm
extern int posix_memalign(void **memptr, size_t alignment, size_t size);
}  // extern "C"
#endif

namespace jm
{

	void System::sleep(int msecs)
	{
#ifdef WIN32
		Sleep(msecs);
#else
		timeval tv;
		tv.tv_sec = msecs / 1000;
		tv.tv_usec = (msecs % 1000) * 1000;
		select(0, nullptr, nullptr, nullptr, &tv);
#endif
	}

	void System::pause(int msecs)
	{
#ifdef WIN32

		// For sleep less than 30ms (except 0), we perform a spinlock to increase the accuracy of sleep() to avoid the win32 scheduler misunderstanding the sleep hint
		if ((msecs < 30) && (msecs >0))
		{
			uint64_t time_start = get_time();
			do
			{
				Sleep(0);
			} while ((get_time() - time_start) < msecs);
		}
		else
		{
			Sleep(msecs);
		}

#else
		timeval tv;
		tv.tv_sec = msecs / 1000;
		tv.tv_usec = (msecs % 1000) * 1000;
		select(0, nullptr, nullptr, nullptr, &tv);
#endif
	}

	int System::get_num_cores()
	{
#ifdef WIN32
		SYSTEM_INFO system_info;
		memset(&system_info, 0, sizeof(SYSTEM_INFO));
		GetSystemInfo(&system_info);
		return system_info.dwNumberOfProcessors;
#else
		long cpus =  -1;
# if defined(_SC_NPROCESSORS_ONLN)
		cpus = sysconf(_SC_NPROCESSORS_ONLN);
# elif defined(_SC_NPROC_ONLN)
		cpus = sysconf(_SC_NPROC_ONLN);
# elif defined(HAVE_SYS_SYSCTL_H)
		int mib[2];
		size_t len;

		mib[0] = CTL_HW;
		mib[1] = HW_NCPU;

		len = sizeof(cpus);
		(void)sysctl(mib, 2, &cpus, &len, NULL, 0);
# endif

		return (cpus < 1)?-1 : static_cast<int>(cpus);

#endif	// WIN32
	}
uint64_t System::get_time()
{
    return (get_microseconds() / 1000);
}

uint64_t System::get_microseconds()
{
#ifdef WIN32
    static LARGE_INTEGER perf_counter;
    static double perf_frequency;
    static bool first_time = true;

    if (first_time)
    {
        LARGE_INTEGER perf_frequency_int64;
        QueryPerformanceFrequency(&perf_frequency_int64);
        perf_frequency = (double)perf_frequency_int64.QuadPart;
        first_time = false;
    }

    // Note on Win32, this looses accuracy after approx 9 days (by 1 to 4 microseconds), due to the precision of a 64bit double.
    // If you require ultra precision, modify this function to use bv::BigInt

    QueryPerformanceCounter(&perf_counter);
    double quad_part = (double)perf_counter.QuadPart;
    return (uint64_t)(((1000000.0 * quad_part) / perf_frequency) + 0.5);
#else
    timeval tv;
    gettimeofday(&tv, nullptr);
    return (uint64_t) tv.tv_sec*(uint64_t) 1000000 + (uint64_t) tv.tv_usec;
#endif
}

std::string System::get_exe_path()
{
#ifdef WIN32
    CHAR exe_filename[_MAX_PATH];
    DWORD len = GetModuleFileName(NULL, exe_filename, _MAX_PATH);
    if (len == 0 || len == _MAX_PATH)
        throw Exception("GetModuleFileName failed!");

    CHAR drive[_MAX_DRIVE], dir[_MAX_DIR];
#ifdef _CRT_INSECURE_DEPRECATE
    _splitpath_s(exe_filename, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
#else
    _splitpath(exe_filename, drive, dir, NULL, NULL);
#endif

    return std::string(drive) + std::string(dir);
#else
    char exe_file[PATH_MAX];
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (mainBundle)
	{
		CFURLRef mainURL = CFBundleCopyBundleURL(mainBundle);

		if (mainURL)
		{
			int ok = CFURLGetFileSystemRepresentation (
				mainURL, (Boolean) true, (UInt8*)exe_file, PATH_MAX
			);

			if (ok)
			{
				return std::string(exe_file) + "/";
			}
		}
	}

	throw Exception("get_exe_path failed");

#else
#ifndef PROC_EXE_PATH
#define PROC_EXE_PATH "/proc/self/exe"
#endif
    int size;
    struct stat sb;
    if (lstat(PROC_EXE_PATH, &sb) < 0)
    {
#ifdef EXTERN___PROGNAME
        char *pathenv, *name, *end;
		char fname[PATH_MAX];
		char cwd[PATH_MAX];
		struct stat sba;

		exe_file[0] = '\0';
		if ((pathenv = getenv("PATH")) != nullptr)
		{
			for (name = pathenv; name; name = end)
			{
				if ((end = strchr(name, ':')))
					*end++ = '\0';
				snprintf(fname, sizeof(fname),
					"%s/%s", name, (char *)__progname);
				if (stat(fname, &sba) == 0) {
					snprintf(exe_file, sizeof(exe_file),
						"%s/", name);
					break;
				}
			}
		}
		// if getenv failed or path still not found
		// try current directory as last resort
		if (!exe_file[0])
		{
			if (getcwd(cwd, sizeof(cwd)) != nullptr)
			{
				snprintf(fname, sizeof(fname),
					"%s/%s", cwd, (char *)__progname);
				if (stat(fname, &sba) == 0)
					snprintf(exe_file, sizeof(exe_file),
						"%s/", cwd);
			}
		}
		if (!exe_file[0])
			throw Exception("get_exe_path: could not find path");
		else
			return std::string(exe_file);
#else
        throw Exception("get_exe_path: proc file system not accesible");
#endif
    }
    else
    {
        size = readlink(PROC_EXE_PATH, exe_file, PATH_MAX);
        if (size < 0)
        {
            throw Exception(strerror(errno));
        }
        else
        {
            exe_file[size] = '\0';
            return std::string(dirname(exe_file)) + "/";
        }
    }
#endif
#endif
}

std::string System::system_call(const std::string &cmd)
{
    if (cmd.empty()) {
        return std::string();
    }

    FILE *fp = popen(cmd.c_str(), "r");
    if (NULL == fp) {
        return std::string();
    }
    char cmd_buff[64] = {0};
    fread(cmd_buff, sizeof(char), sizeof(cmd_buff), fp);
    pclose(fp);

    return std::string(cmd_buff);
}

void System::system_call_async(const std::string &cmd, std::function<void(bool)> callback_func)
{
    if (cmd.empty()) {
        return;
    }

    std::thread([=]() {
        FILE *fp = popen(cmd.c_str(), "r");
        callback_func(NULL != fp);
        pclose(fp);
    }).detach();
}
}
