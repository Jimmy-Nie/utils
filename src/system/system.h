
#pragma once

#include <mutex>
#include <vector>

namespace jm {
/// \addtogroup bvcutils_System bvcutils System
/// \{

/// \brief General system helper functions.
class System {
public:
    /// \brief Get the current time (since system boot), in milliseconds.
    static uint64_t get_time();

    /// \brief Get the current time microseconds.
    static uint64_t get_microseconds();

    /// \brief Return the number of CPU cores
    static int get_num_cores();

    /// \brief Sleep for 'millis' milliseconds.
    ///
    /// It is possible for this function to sleep for more than millis, depending on the OS scheduler.
    /// If you require a more accurate sleep, consider pause.
    static void sleep(int millis);

    /// \brief Pause for 'millis' milliseconds.
    ///
    /// This function acts as sleep, but the function "may" perform a spinlock on some operating systems
    /// to give a more accurate pause. This may have a side effect of causing 100% cpu usage.
    /// If you do not require an accurate pause, use sleep instead.
    static void pause(int millis);

    /// \brief Returns the full dirname of the executable that started this
    /// \brief process (aka argv[0])
    /** <p>This is necessary since when programms are started from
            the PATH there is no clean and simple way to figure out
            the location of the data files, thus information is read
            from <tt>/proc/self/exe</tt> under GNU/Linux and from
            GetModuleFileName() on Win32.</p>
            \return full dirname of the executable, trailing slash is included*/
    static std::string get_exe_path();

    ///\brief: system call the shell cmd
    /// also can exec shell cmd async, register the call back function
    ///\return: if system call succeed, return the shell cmd result;
    /// otherwise return empty string
    static std::string  system_call(const std::string &cmd);
    static void         system_call_async(const std::string &cmd, std::function<void(bool)> callback_func);
};

/// \}
} // namespace jm
