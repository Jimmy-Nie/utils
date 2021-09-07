#pragma once

#include <memory>
#include <string>
#include "system/endian.h"
#include "exception/exception.h"

namespace jm
{

class IODeviceProvider;

/// \brief I/O Device interface.
///
/// This class can store basic datatypes and retain portability (using the specified endian mode)\n
///    The supported datatypes are: int64_t, int32_t, int16_t and int8_t\n
///    The std::string datatype is supported - Using Size(int32_t), Charactor Data (std::string characters)
class IODevice
{
public:
    enum SeekMode
    {
        seek_set,        /// \brief Set to a specific value from the start
        seek_cur,        /// \brief Set relative to the current position
        seek_end        /// \brief Set to a specific value from the end (use negative position)
    };

    /// \brief Constructs a null instance
    IODevice() = delete;
    IODevice(IODeviceProvider* provider);
    ~IODevice();

    /// \brief Returns true if this object is invalid.
    bool is_null() const;

    /// \brief Throw an exception if this object is invalid.
    void throw_if_null() const;

    /// \brief Returns the size of data stream.(in char type)
    /// Returns SIZE_MAX if the size is unknown.</p>
    /// \return The size (SIZE_MAX if size is unknown)*/
    size_t get_size() const;

    /// \brief Returns the position in the data stream.
    /** <p>Returns SIZE_MAX if the position is unknown.</p>
        \return The size (SIZE_MAX if position is unknown)*/
    size_t get_position() const;

    /// \brief Returns true if the input source is in little endian mode.
    /** \return true if little endian*/
    bool is_little_endian() const;

    /// \brief Returns the provider for this object
    const IODeviceProvider* get_provider() const;

    /// \brief Returns the provider for this object
    IODeviceProvider* get_provider();

    /// \brief Send data to device.
    /** If the device databuffer is too small, it will be extended (ie grow memory block size or file size)
        \param data Data to send
        \param len Length to send
        \param send_all true to send all the data. false = send part of the data, if it would block
        \return size of data sent*/
    size_t send(const void *data, size_t len, bool send_all = true);

    /// \brief Receive data from device.
    ///
    /// \param data Data to receive
    /// \param len Length to receive
    /// \param receive_all true to receive all the data. false = receive part of the data, if it would block
    ///
    /// \return size of data received
    size_t receive(void *data, size_t len, bool receive_all = true);

    /// \brief Peek data from device (data is left in the buffer).
    ///
    /// \param data Data to receive
    /// \param len Maximum length of data to receive
    /// \return size of data received.
    size_t peek(void *data, size_t len);

    /// \brief Seek in data stream.
    ///
    /// \param position Position to use (usage depends on the seek mode)
    /// \param mode Seek mode
    /// \return false = Failed
    bool seek(int position, SeekMode mode = seek_set);

    /// \brief Alias for receive(data, len, receive_all)
    ///
    /// \param data Data to receive
    /// \param len Length to receive
    /// \param receive_all true to receive all the data. false = receive part of the data, if it would block
    ///
    /// \return size of data received
    size_t read(void *data, size_t len, bool receive_all = true);

    /// \brief Alias for send(data, len, send_all)
    ///
    /// \param data Data to send
    /// \param len Length to send
    /// \param send_all true to send all the data. false = send part of the data, if it would block
    ///
    /// \return size of data sent
    size_t write(const void *data, size_t len, bool send_all = true);

    /// \brief Changes input data endianess to the local systems mode.
    void set_system_mode();

    /// \brief Changes input data endianess to big endian mode. (Default is little endian)
    void set_big_endian_mode();

    /// \brief Changes input data endianess to little endian mode. This is the default setting.
    void set_little_endian_mode();

    /// \brief Writes a var to output source.
    ///
    /// \param data Integer to write
    template <typename T>
    void write_var(T data)
    {
        if (is_little_endian()) {
            Endian::swap_to_little_endian(&data, sizeof(data));
        } else {
            Endian::swap_to_big_endian(&data, sizeof(data));
        }
        write(&data, sizeof(T));
    }

    /// \brief  Writes a string to the output source.
    ///
    /// \param str String to write
    ///
    /// The binary format written to the output source is first an uint32 telling the length of the
    /// string, and then the string itself.
    void write_string_a(const std::string &str);

    /// \brief Writes a nul terminated string to the output source.
    ///
    /// \param str String to write
    ///
    /// The binary format written to the output source is the string content followed by the NUL character.
    void write_string_nul(const std::string &str);

    /// \brief Writes a text string to the output source.
    ///
    /// \param str String to write
    ///
    /// The binary format written to the output source is the string content appended with a native newline.
    /// On Windows the newline is CR+LF sequence and on other platforms it is only LF character.
    /// This function is intended for use with text files.
    void write_string_text(const std::string &str);

    /// \brief Reads a var from input source.
    /** \return The integer read.*/
    template <typename T>
    T read_var()
    {
        T answer;
        if (read(&answer, sizeof(T)) != sizeof(T))
            throw Exception("IODevice::read_var failed");
        if (is_little_endian()) {
            Endian::swap_to_little_endian(&answer, sizeof(answer));
        } else {
            Endian::swap_to_big_endian(&answer, sizeof(answer));
        }
        return answer;
    }

    /// \brief Reads a string from the input source.
    /** <p>The binary format expected in the input source is first an uint32 telling the length of the
        string, and then the string itself.</p>
        \return The string read.*/
    std::string read_string_a();

    /// \brief Reads a nul terminated string from the input source.
    /** The binary format expected in the input source is a nul terminated string.
        (The NUL termintor is read, so that the file position is set after the NUL)
        \return The string read.*/
    std::string read_string_nul();

    /// \brief  Reads a string from the input source where the source is a text file.
    ///
    /// After reading the input source up to "read_until_chars", the file position is set to the first character that was not read.
    /// If the file contains NUL characters, then the input is read up to the NUL character, and the file position is set AFTER the NUL)
    ///
    /// \param skip_initial_chars Ignore any of these characters at the start of the string. NULL = Do not ignore any characters
    /// \param read_until_chars Read from the input until any of these characters are found. NULL = Read until the end of the file
    /// \param allow_eof Allow EOF
    /// \return The string read.
    std::string read_string_text(const char *skip_initial_chars, const char *read_until_chars, bool allow_eof = true);

    /// \brief Create a new IODevice referencing the same resource.
    IODevice duplicate();

private:
    bool little_endian_ = true;
    IODeviceProvider* provider_ = nullptr;
};

}
