
#include "iodata/iodevice.h"
#include "iodata/iodevice_provider.h"
#include "system/endian.h"
#include "exception/exception.h"
namespace jm
{

IODevice::IODevice(IODeviceProvider* provider) : provider_(provider)
{
}

IODevice::~IODevice()
{
}

bool IODevice::is_null() const {
    return !provider_;
}

void IODevice::throw_if_null() const
{
    if (!provider_)
        throw Exception("IODevice is null");
}

size_t IODevice::get_size() const
{
    if (provider_)
        return provider_->get_size();
    return -1;
}

size_t IODevice::get_position() const
{
    if (provider_)
        return provider_->get_position();
    return -1;
}

bool IODevice::is_little_endian() const
{
    return little_endian_;
}

const IODeviceProvider* IODevice::get_provider() const
{
    throw_if_null();
    return provider_;
}

IODeviceProvider* IODevice::get_provider()
{
    throw_if_null();
    return provider_;
}

size_t IODevice::send(const void *data, size_t len, bool send_all)
{
    if (provider_)
        return provider_->send(data, len, send_all);
    return -1;
}

size_t IODevice::receive(void *data, size_t len, bool receive_all)
{
    if (provider_)
        return provider_->receive(data, len, receive_all);
    return -1;
}

size_t IODevice::peek(void *data, size_t len)
{
    if (provider_)
        return provider_->peek(data, len);
    return -1;
}

bool IODevice::seek(int position, SeekMode mode)
{
    if (provider_)
        return provider_->seek(position, mode);
    return false;
}

size_t IODevice::read(void *data, size_t len, bool receive_all)
{
    return receive(data, len, receive_all);
}

size_t IODevice::write(const void *data, size_t len, bool send_all)
{
    return send(data, len, send_all);
}

void IODevice::set_system_mode()
{
    little_endian_ = !Endian::is_system_big();
}

void IODevice::set_big_endian_mode()
{
    little_endian_ = false;
}

void IODevice::set_little_endian_mode()
{
    little_endian_ = true;
}

void IODevice::write_string_a(const std::string &str)
{
    size_t size = str.length();
    write_var<int32_t>(int32_t(size));
    write(str.data(), size);
}

void IODevice::write_string_nul(const std::string &str)
{
    write(str.c_str(), str.length() + 1);
}

void IODevice::write_string_text(const std::string &str)
{
    write(str.data(), str.length());
#ifdef WIN32
    write("\r\n", 2);
#else
    write("\n", 1);
#endif
}

std::string IODevice::read_string_a()
{
    size_t size = size_t(read_var<int32_t>());

    auto str = new char[size];
    try
    {
        read(str, size);

        std::string ret(str, size);
        delete[] str;

        return ret;
    }
    catch (...)
    {
        delete[] str;
        throw;
    }
}

std::string IODevice::read_string_nul()
{
    return (read_string_text(nullptr, nullptr, false));
}

std::string IODevice::read_string_text(const char *skip_initial_chars, const char *read_until_chars, bool allow_eof)
{
    const size_t buffer_size = 64;
    char buffer[buffer_size];
    size_t read_size;
    size_t size = 0;
    bool find_flag = true;
    bool null_found = false;
    size_t current_position = get_position();

    // Skip initial unwanted chars
    if (skip_initial_chars)
    {
        while (find_flag)	// For the entire file
        {
            read_size = receive(buffer, buffer_size, true);

            char *dptr = buffer;
            for (size_t cnt = 0; cnt != read_size; ++cnt, dptr++)	// Search the buffer
            {
                char letter = *dptr;
                bool match_flag = false;
                for (const char *rptr = skip_initial_chars; *rptr; rptr++)	// Search the initial chars array
                {
                    if (letter == *rptr)
                    {
                        match_flag = true;
                        break;
                    }
                }
                if (!match_flag)	// Not found a match
                {
                    find_flag = false;
                    break;
                }
                current_position++;	// Skip the char
            }

            if ((find_flag) && (read_size != buffer_size))	// eof found
            {
                if (!allow_eof)
                {
                    throw Exception("IODevice_Datafile::read_string_text() failed - end of file");
                }
                break;
            }
        }

        seek(int(current_position), seek_set);	// Set new position
    }

    find_flag = true;

    // Calculate the length of the string
    while (find_flag)
    {
        read_size = receive(buffer, buffer_size, true);
        char *dptr = buffer;
        for (size_t cnt = 0; cnt != read_size; ++cnt, dptr++)
        {
            char letter = *dptr;
            // Treat NUL as a special terminating character
            if (letter == 0)
            {
                null_found = true;
                find_flag = false;
                break;
            }
            bool match_flag = false;
            if (read_until_chars)
            {
                for (const char *rptr = read_until_chars; *rptr; rptr++)	// Search the read until chars array
                {
                    if (letter == *rptr)
                    {
                        match_flag = true;
                        break;
                    }
                }
                if (match_flag)	// Found a match
                {
                    find_flag = false;
                    break;
                }
            }

            size++;
        }

        if ((find_flag) && (read_size != buffer_size))	// eof found
        {
            if (!allow_eof)
            {
                throw Exception("IODevice_Datafile::read_string_text() failed - end of file");
            }
            break;
        }
    }
    seek(int(current_position), seek_set);

    // Read the string, now that we know its length

    auto str = new char[size];
    try
    {
        read(str, size);
        std::string ret(str, size);
        if (null_found)
        {
            read(buffer, 1);	// Read the NUL terminator
        }
        delete[] str;

        return ret;
    }
    catch (...)
    {
        delete[] str;
        throw;
    }
}

IODevice IODevice::duplicate()
{
    return IODevice(provider_->duplicate());
}

}
