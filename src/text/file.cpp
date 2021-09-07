#include "file.h"
#include "text/path.h"
#include "exception/exception.h"
#include "iodata/src/iodevice_provider_file.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace jm
{

std::string File::read_text(const std::string &filename)
{
    File file(filename);
    size_t file_size = file.get_size();
    std::vector<char> text;
    text.resize(file_size + 1);
    text[file_size] = 0;
    if (file_size)
        file.read(&text[0], file_size);
    file.close();
    if (file_size)
        return std::string(&text[0]);
    else
        return std::string();
}

DataBuffer File::read_bytes(const std::string &filename)
{
    File file(filename);
    DataBuffer buffer(file.get_size());
    file.read(buffer.get_data(), buffer.get_size());
    file.close();
    return buffer;
}

void File::write_text(const std::string &filename, const std::string &text, bool write_bom)
{
    File file(filename, create_always, access_write);
    if (write_bom)
    {
        unsigned char bom[3] = { 0xef, 0xbb, 0xbf };
        file.write(bom, 3);
    }
    file.write(text.data(), text.length());
    file.close();
}

void File::write_bytes(const std::string &filename, const DataBuffer &bytes)
{
    File file(filename, create_always, access_write);
    file.write(bytes.get_data(), bytes.get_size());
    file.close();
}

File::File()
    : IODevice(new IODeviceProvider_File())
{
}

File::File(
    const std::string &filename)
    : IODevice(new IODeviceProvider_File(Path::normalize(filename, Path::path_type_file), open_always, access_read, share_all, 0))
{
}

File::File(
    const std::string &filename,
    OpenMode open_mode,
    unsigned int access,
    unsigned int share,
    unsigned int flags)
    : IODevice(new IODeviceProvider_File(Path::normalize(filename, Path::path_type_file), open_mode, access, share, flags))
{
}
File::~File()
{
}

bool File::open(
    const std::string &filename)
{
    auto provider = dynamic_cast<IODeviceProvider_File*>(get_provider());
    return provider->open(Path::normalize(filename, Path::path_type_file), open_always, access_read, share_all, 0);
}

bool File::open(
    const std::string &filename,
    OpenMode open_mode,
    unsigned int access,
    unsigned int share,
    unsigned int flags)
{
    auto provider = dynamic_cast<IODeviceProvider_File*>(get_provider());
    return provider->open(Path::normalize(filename, Path::path_type_file), open_mode, access, share, flags);
}

void File::close()
{
    auto provider = dynamic_cast<IODeviceProvider_File*>(get_provider());
    provider->close();
}

///====================================================================================================================
/// Static function
///====================================================================================================================
void File::copy_file(const std::string &from, const std::string &to, bool copy_always)
{
#ifdef WIN32
    BOOL result = CopyFile(from.c_str(), to.c_str(), copy_always ? FALSE : TRUE);
    if (result == FALSE)
        throw Exception("Unable to copy file");
#else
    while (!copy_always)
    {
        try
        {
            File input_file(to);
        }
        catch (const Exception&)
        {
            break;
        }

        throw Exception("Destination file already exists");
    }

    File input_file(from);
    File output_file(to, File::create_always, File::access_read_write);
    char buffer[16*1024];
    while (true)
    {
        int bytes_read = input_file.read(buffer, 16*1024);
        if (bytes_read <= 0)
            break;

        int bytes_written = output_file.write(buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            output_file.close();
            File::delete_file(to);
            throw Exception("Unable to copy file");
        }
    }
#endif
}

void File::delete_file(const std::string &filename)
{
#ifdef WIN32
    BOOL result = DeleteFile(filename.c_str());
    if (result == FALSE)
        throw Exception("Unable to delete file");
#else
    std::string filename_local8 = filename;
    int result = unlink(filename_local8.c_str());
    if (result == -1)
        throw Exception("Unable to delete file");
#endif
}

bool File::file_exists(const std::string &filename)
{
#ifdef WIN32
    HANDLE file = CreateFile(
        filename.c_str(),	/* lpFileName */
        0,					/* dwDesiredAccess */
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,	/* dwShareMode */
        0,					/* lpSecurityAttributes */
        OPEN_EXISTING,		/* dwCreationDisposition */
        0,					/* dwFlagsAndAttributes */
        0);					/* hTemplateFile */

    if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
    return file != INVALID_HANDLE_VALUE;

    //	return (GetFileAttributes(filename.c_str()) != INVALID_FILE_ATTRIBUTES);
#else
    struct stat stFileInfo;
    return (stat(filename.c_str(), &stFileInfo) == 0);
#endif
}

}
