
#pragma once

#include "iodata/iodevice.h"
#include "iodata/databuffer.h"

namespace jm
{

/// \brief File I/O device.
class File : public IODevice
{
public:
    /// \brief Access flags.
    /** <p>File access types available.</p>*/
    enum AccessFlags
    {
        access_read = 1,            /// \brief Generic read access.
        access_write = 2,           /// \brief Generic write access.
        access_read_write = access_read + access_write        /// \brief Generic read write access.
    };

    /// \brief File sharing flags.
    enum ShareFlags
    {
        share_read = 1,        /// \brief Allow others to open the file for reading.
        share_write = 2,        /// \brief Allow others to open the file for writing.
        share_delete = 4,        /// \brief Allow others to delete the file.
        share_all = share_read + share_write + share_delete        /// \brief All other sharing flags combined.
    };

    /// \brief File opening modes.
    enum OpenMode
    {
        open_always,                    /// \brief Open file or create it if it does not exist.
        open_existing,                  /// \brief Open existing file.  Fails if it does not exist.
        open_existing_truncate,         /// \brief Open existing file and truncate it.
        create_always,                  /// \brief Create file, even if it already exists.
        create_new                      /// \brief Create a new file.  Fails if it already exists.
    };

    /// \brief Optimization Flags.
    enum Flags
    {
        flag_write_through = 1,
        flag_no_buffering = 2,
        flag_random_access = 4,
        flag_sequential_scan = 8
    };

    /// \brief Constructs a file object.
    File();

    /// \brief Constructs a file object read only
    ///
    /// PathHelp::normalize(filename, PathHelp::path_type_file) is called
    File(const std::string &filename);

    File(
        const std::string &filename,
        OpenMode mode,
        unsigned int access,
        unsigned int share = share_all,
        unsigned int flags = 0);

    ~File();

    /// \brief Opens a file read only.
    ///
    /// PathHelp::normalize(filename, PathHelp::path_type_file) is called
    bool open(const std::string &filename);
    bool open(
        const std::string &filename,
        OpenMode mode,
        unsigned int access,
        unsigned int share = share_all,
        unsigned int flags = 0);

    /// \brief Close file.
    void close();

public:
    /// \brief Loads an UTF-8 text file into a string.
    static std::string read_text(const std::string &filename);

    /// \brief Loads an file into a byte buffer.
    static DataBuffer read_bytes(const std::string &filename);

    /// \brief Saves an UTF-8 text string to file.
    static void write_text(const std::string &filename, const std::string &text, bool write_bom = false);

    /// \brief Saves a byte buffer to file.
    static void write_bytes(const std::string &filename, const DataBuffer &bytes);

    /// \brief Copy a file.
    ///
    /// \param from Where to copy from
    /// \param to Where to copy to
    /// \param copy_always true = Copy, even if destination already exists. false = Do not copy if destination exists (throw a Exception)
    static void copy_file(const std::string &from, const std::string &to, bool copy_always);

    /// \brief Delete a file.
    ///
    /// \param filename File to delete
    static void delete_file(const std::string &filename);

    /// \brief Check if a file exists.
    ///
    /// \param filename File to check for existance
    static bool file_exists(const std::string &filename);
};

}
