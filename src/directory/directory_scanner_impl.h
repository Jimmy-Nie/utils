
#pragma once

#include <string>

namespace jm
{

class DirectoryScanner_Impl
{
public:
    virtual ~DirectoryScanner_Impl() { }

    virtual bool scan(const std::string& pathname) = 0;
    virtual bool scan(const std::string& pathname, const std::string& pattern) = 0;

    /// \brief Returns the path of the directory being scanned.
    virtual std::string get_directory_path() = 0;

    /// \brief Returns the name of the current found file.
    virtual std::string get_name() = 0;

    /// \brief Returns the size of the current found file.
    virtual int get_size() = 0;

    /// \brief Returns the name of the current found file, including the directory path.
    virtual std::string get_pathname() = 0;

    /// \brief Returns true if filename is a directory.
    virtual bool is_directory() = 0;

    /// \brief Returns true if filename is hidden.
    virtual bool is_hidden() = 0;

    /// \brief Returns true if file is readable by current user.
    virtual bool is_readable() = 0;

    /// \brief Returns true if file is writable by current user.
    virtual bool is_writable() = 0;

    /// \brief Find next file in directory scan. Returns false if no more files was found.
    virtual bool next() = 0;
};

}
