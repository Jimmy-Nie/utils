#pragma once

#include <string>
namespace jm {

/// \brief Directory utility class
class Directory {
public:
    /// \brief Create directory.
    ///
    /// \param dir_name = Directory name for create.
    /// \param recursive = If true, function will create all directories in the path, otherwise only the last directory
    /// \return true on success or false on error.
    static bool create(const std::string &dir_name, bool recursive = false);

    /// \brief Remove directory.
    ///
    /// \param dir_name = Directory name for delete.
    /// \param delete_files = If true, function will delete files.
    /// \param delete_sub_directories = If true, function will delete subdirectories too.
    /// \return true on success or false on error.
    static bool remove(const std::string &dir_name, bool delete_files = false, bool delete_sub_directories = false);

    /// \brief Rename directory.
    ///
    /// \param old_name = Old name of the directory to be renamed.
    /// \param new_name = New directory name.
    /// \return true on success or false on error.
    static bool rename(const std::string &old_name, const std::string &new_name);

    /// \brief Change current directory.
    ///
    /// \param dir_name = Directory name to change to.
    /// \return true on success or false on error.
    static bool set_current(const std::string &dir_name);

    /// \brief Get current directory.
    ///
    /// \return the current directory path.
    static std::string get_current();
};

} // namespace bv
