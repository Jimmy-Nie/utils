
#include "directory.h"
#include "directory_scanner.h"
#include "path.h"
#include "share/exception.h"

#if defined(__MINGW32__) && !defined(_WIN32_IE)
#define _WIN32_IE 0x0500
#endif

#ifndef WIN32
#include <cstdio>
#include <pwd.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 256 // TODO: Fixme - Paths should not have a limit
#endif

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#else
#include <shlobj.h>

#ifndef MAX_PATH
#define MAX_PATH _MAX_PATH
#endif

#ifdef _MSC_VER
#pragma comment(lib, "shell32.lib")
#endif
#endif

#include <sys/stat.h>
#include <sys/types.h>

namespace jm {

bool Directory::create(const std::string &dir_name, bool recursive)
{
    if (dir_name.empty())
        return false;

    std::string full_path = Path::add_trailing_slash(dir_name);

    bool result = true;
    if (recursive) {
        for (std::string::size_type pos = full_path.find_first_of("\\/"); pos != std::string::npos;
             pos                        = full_path.find_first_of("\\/", pos + 1)) {
            std::string path = full_path.substr(0, pos);
#ifdef WIN32
            result = CreateDirectory(path.c_str(), NULL) != 0;
#else
            result = mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
#endif
        }
    } else {
#ifdef WIN32
        result = CreateDirectory(full_path.c_str(), NULL) != 0;
#else
        result = mkdir(full_path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
#endif
    }

    return result;
}

bool Directory::remove(const std::string &dir_name, bool delete_files, bool delete_sub_directories)
{
    if (dir_name.empty())
        return false;

    std::string full_path = Path::add_trailing_slash(dir_name);

    if (delete_files || delete_sub_directories) {
        DirectoryScanner scanner;

        if (!scanner.scan(full_path))
            return false;

        bool scan_successful = scanner.next();
        while (scan_successful) {
            // if found subdirectory, try remove it, also checking for "." and "..", because they are unremovable
            if (scanner.is_directory() && delete_sub_directories && scanner.get_name() != "."
                && scanner.get_name() != "..") {
                // FIXME: directory scanner locks directory, so it can't be removed, this is workaround
                std::string sub_dir_path = scanner.get_pathname();
                scan_successful          = scanner.next();

                // delete files in subdirectory
                if (!Directory::remove(sub_dir_path, delete_files, delete_sub_directories))
                    return false;
            } else if (!scanner.is_directory() && delete_files) {
#ifdef WIN32
                if (DeleteFile(scanner.get_pathname().c_str()) == 0)
                    return false;
#else
                if (::remove(scanner.get_pathname().c_str()) != 0)
                    return false;
#endif
                scan_successful = scanner.next();
            } else {
                scan_successful = scanner.next();
            }
        }
    }

    // finally remove the directory (or subdirectory if in recursion)
#ifdef WIN32
    return RemoveDirectory(full_path.c_str()) != 0;
#else
    return rmdir(full_path.c_str()) == 0;
#endif
}

bool Directory::rename(const std::string &old_name, const std::string &new_name)
{
#ifdef WIN32
    return MoveFile(old_name.c_str(), new_name.c_str()) != 0;
#else
    return ::rename(old_name.c_str(), new_name.c_str()) == 0;
#endif
}

bool Directory::set_current(const std::string &dir_name)
{
#ifdef WIN32
    return SetCurrentDirectory(dir_name.c_str()) != 0;
#else
    return chdir(dir_name.c_str()) == 0;
#endif
}

std::string Directory::get_current()
{
#ifdef WIN32
    DWORD len = GetCurrentDirectory(0, NULL);
    if (len == 0)
        throw Exception("GetCurrentDirectory failed!");

    std::vector<TCHAR> path(len);
    if (GetCurrentDirectory(len, &path[0]) == 0)
        throw Exception("GetCurrentDirectory failed!");
    return &path[0];
#else
    char cwd_buffer[MAX_PATH];
    if (getcwd(cwd_buffer, MAX_PATH) == nullptr)
        throw Exception("Working dir is more than legal length !");
    return cwd_buffer;
#endif
}
} // namespace bv
