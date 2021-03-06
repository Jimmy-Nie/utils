#include "path.h"
#include "exception/exception.h"

#ifdef WIN32
#include <direct.h>
#include <tchar.h>
#include <wchar.h>
#endif

#ifndef WIN32
#include <cstring>
#include <unistd.h>
#endif

namespace jm {
std::string Path::make_absolute(const std::string &base_path, const std::string &relative_path, PathType path_type)
{
    std::string base     = normalize(base_path, path_type);
    std::string relative = normalize(relative_path, path_type);
    if (path_type == path_type_file) {
        if (!is_absolute(base, path_type_file)) {
            // make base_path absolute using current drive and directory
#ifdef WIN32
            CHAR absolute_base[MAX_PATH];
            memset(absolute_base, 0, sizeof(CHAR) * MAX_PATH);
            if (_fullpath(absolute_base, base.c_str(), MAX_PATH) == 0)
                throw Exception(fmt::format("Unable to make base path absolute: {}", base_path));
            base = std::string(absolute_base);
#else
            char working_dir[1024];
            memset(working_dir, 0, 1024);
            if (getcwd(working_dir, 1024) == nullptr)
                throw Exception("Unable to get current working directory!");
            base = add_trailing_slash(working_dir, path_type) + base;
#endif
        }

        std::string base_location     = get_location(base_path, path_type_file);
        std::string relative_location = get_location(relative_path, path_type_file);
        if (relative_location.empty() || relative_location == base_location) {
            if (is_absolute(relative, path_type)) {
                if (relative_location.empty())
                    return base_location + relative;
                else
                    return relative;
            }
            std::string absolute = add_trailing_slash(base, path_type) + relative.substr(relative_location.length());
            return normalize(absolute, path_type);
        } else {
#ifdef WIN32
            if (is_absolute(relative, path_type))
                return relative;

            if (relative_location.length() == 2 && relative_location[1] == ':') {
                int drive = 0;
                if (relative_location[0] >= 'A' && relative_location[0] <= 'Z')
                    drive = relative_location[0] - 'A' + 1;
                else if (relative_location[0] >= 'a' && relative_location[0] <= 'z')
                    drive = relative_location[0] - 'a' + 1;
                else
                    throw Exception(fmt::format("Invalid drive: {}", relative_location));
                CHAR working_dir[MAX_PATH];
                memset(working_dir, 0, sizeof(CHAR) * MAX_PATH);
                if (_getdcwd(drive, working_dir, MAX_PATH) == 0)
                    throw Exception(fmt::format("Unable to get current working directory for {}!", relative_location));

                return add_trailing_slash(working_dir, path_type) + relative.substr(relative_location.length());
            } else {
                return relative; // UNC path
            }
#else
            throw Exception("Error in Path::make_absolute");
#endif
        }
    } else {
        if (is_absolute(relative, path_type))
            return relative;
        std::string absolute = add_trailing_slash(base, path_type) + relative;
        return normalize(absolute, path_type);
    }
}

std::string Path::make_relative(const std::string &base_path, const std::string &absolute_path, PathType path_type)
{
    std::string base     = add_trailing_slash(normalize(base_path, path_type), path_type);
    std::string absolute = normalize(absolute_path, path_type);

    if (path_type == path_type_file) {
        std::string base_location     = get_location(base, path_type_file);
        std::string absolute_location = get_location(absolute, path_type_file);

        if (is_relative(base, path_type)) {
#ifdef WIN32
            if (base_location.length() == 2 && base_location[1] == ':') {
                int drive = 0;
                if (base_location[0] >= 'A' && base_location[0] <= 'Z')
                    drive = base_location[0] - 'A' + 1;
                else if (base_location[0] >= 'a' && base_location[0] <= 'z')
                    drive = base_location[0] - 'a' + 1;
                else
                    throw Exception(fmt::format("Invalid drive: %1", base_location));
                CHAR working_dir[MAX_PATH];
                memset(working_dir, 0, sizeof(CHAR) * MAX_PATH);
                if (_getdcwd(drive, working_dir, MAX_PATH) == 0)
                    throw Exception(fmt::format("Unable to get current working directory for %1!", base_location));

                base = add_trailing_slash(working_dir, path_type) + base;
            } else if (base_location.empty()) {
                CHAR working_dir[MAX_PATH];
                memset(working_dir, 0, sizeof(CHAR) * MAX_PATH);
                if (GetCurrentDirectory(MAX_PATH, working_dir) == FALSE)
                    throw Exception(fmt::format("Unable to get current working directory for %1!", base_location));

                base = add_trailing_slash(working_dir, path_type) + base;
            } else {
                throw Exception(fmt::format("Error in make_relative with base path: %1", base_path));
            }
#else
            char working_dir[1024];
            memset(working_dir, 0, 1024);
            if (getcwd(working_dir, 1024) == nullptr)
                throw Exception("Unable to get current working directory!");
            base = add_trailing_slash(working_dir, path_type) + base;
#endif
        }
        if (is_relative(absolute, path_type)) {
#ifdef WIN32
            if (absolute_location.length() == 2 && absolute_location[1] == ':') {
                int drive = 0;
                if (absolute_location[0] >= 'A' && absolute_location[0] <= 'Z')
                    drive = absolute_location[0] - 'A' + 1;
                else if (absolute_location[0] >= 'a' && absolute_location[0] <= 'z')
                    drive = absolute_location[0] - 'a' + 1;
                else
                    throw Exception(fmt::format("Invalid drive: %1", absolute_location));
                CHAR working_dir[MAX_PATH];
                memset(working_dir, 0, sizeof(WCHAR) * MAX_PATH);
                if (_getdcwd(drive, working_dir, MAX_PATH) == 0)
                    throw Exception(fmt::format("Unable to get current working directory for %1!", absolute_location));

                absolute = add_trailing_slash(working_dir, path_type) + absolute;
            } else if (absolute_location.empty()) {
                CHAR working_dir[MAX_PATH];
                memset(working_dir, 0, sizeof(CHAR) * MAX_PATH);
                if (GetCurrentDirectory(MAX_PATH, working_dir) == FALSE)
                    throw Exception(fmt::format("Unable to get current working directory for %1!", absolute_location));

                absolute = add_trailing_slash(working_dir, path_type) + absolute;
            } else {
                throw Exception(fmt::format("Error in make_relative with absolute path: {}", absolute_path));
            }
#else
            char working_dir[1024];
            memset(working_dir, 0, 1024);
            if (getcwd(working_dir, 1024) == nullptr)
                throw Exception("Unable to get current working directory!");
            absolute = add_trailing_slash(working_dir, path_type) + absolute;
#endif
        }

        base_location     = get_location(base, path_type_file);
        absolute_location = get_location(absolute, path_type_file);
        if (absolute_location == base_location)
            return absolute_path;
    }

    if (is_relative(base, path_type))
        throw Exception("Relative path " + base_path + " used as base path for make_relative");
    if (is_relative(absolute, path_type))
        throw Exception("Relative path " + absolute_path + " used as absolute path for make_relative");

    std::string relative;
    std::string relative_end;

    bool                   differs   = false;
    std::string::size_type start_pos = 0, end_pos = 0;
    while (true) {
        if (path_type == path_type_file) {
            end_pos = base.find_first_of("\\/", start_pos);
        } else {
            end_pos = base.find('/', start_pos);
        }
        if (end_pos == std::string::npos)
            break;

        if (!differs) {
            std::string base_element     = base.substr(start_pos, end_pos - start_pos + 1);
            std::string absolute_element = absolute.substr(start_pos, end_pos - start_pos + 1);

            bool same_element = false;
            if (path_type == path_type_file) {
#ifdef WIN32
                same_element = (Strings::compare(base_element, absolute_element, true) == 0);
#else
                same_element = (base_element == absolute_element);
#endif
            } else {
                same_element = (base_element == absolute_element);
            }

            if (!same_element) {
                relative_end = absolute.substr(start_pos);
                differs      = true;
            } else {
                relative_end = absolute.substr(end_pos + 1);
            }
        }

        if (differs) {
            if (path_type_file) {
#ifdef WIN32
                relative += "..\\";
#else
                relative += "../";
#endif
            } else {
                relative += "../";
            }
        }

        start_pos = end_pos + 1;
    }

    return relative + relative_end;
}

bool Path::is_absolute(const std::string &path, PathType path_type)
{
    if (path_type == path_type_file) {
#ifdef WIN32
        if (path.length() < 3)
            return false;
        if (path[1] == ':' && (path[2] == '\\' || path[2] == '/'))
            return true;
        if (path[0] == '\\' && path[1] == '\\')
            return true;
        return false;
#else
        if (path.length() < 1)
            return false;
        if (path[0] == '/')
            return true;
        if (path[0] == '\\')
            return true;
        return false;
#endif
    } else {
        if (path.length() < 1)
            return false;
        if (path[0] == '/')
            return true;
        return false;
    }
}

bool Path::is_relative(const std::string &path, PathType path_type)
{
    return !is_absolute(path, path_type);
}

std::string Path::normalize(const std::string &input_path, PathType path_type)
{
    if (input_path.empty())
        return std::string();

    std::string location = get_location(input_path, path_type);
    std::string path     = input_path.substr(location.length());

    bool ends_in_slash = false;

    if (input_path[input_path.size() - 1] == '/' || input_path[input_path.size() - 1] == '\\') {
        ends_in_slash = true;
        if (input_path.size() == 1) {
#ifdef WIN32
            if (path_type == path_type_file) {
                return "\\";
            }
#endif
            return "/";
        }
    }

    std::vector<std::string> elements;
    bool                     absolute = false;
    int                      level    = 0;
    std::string::size_type   pos = 0, last_pos = 0;

    while (true) {
        pos = path.find_first_of("/\\", last_pos);

        if (pos == std::string::npos)
            pos = path.length();

        if (pos == 0) {
            absolute = true;
        } else {
            std::string element = path.substr(last_pos, pos - last_pos);
            if (element.empty() && pos != path.length())
                throw Exception("Unable to normalize invalid path " + input_path);

            if (element.empty()) {
            } else if (element == ".") {
            } else if (element == "..") {
                level--;
                if (!elements.empty()) {
                    if (elements[elements.size() - 1] != "..")
                        elements.erase(elements.begin() + elements.size() - 1);
                    else
                        elements.push_back("..");
                } else {
                    elements.push_back("..");
                }

                if (absolute && level < 0)
                    throw Exception("Unable to normalize invalid path " + input_path);
            } else {
                elements.push_back(element);
                level++;
            }
        }

        if (pos == path.length())
            break;

        last_pos = pos + 1;
    }

    std::string normalized_path;

    if (absolute) {
#ifdef WIN32
        if (path_type == path_type_file)
            normalized_path += "\\";
        else
            normalized_path += "/";
#else
        normalized_path += "/";
#endif
    }

    for (auto &element : elements) {
#ifdef WIN32
        if (path_type == path_type_file)
            normalized_path += element + "\\";
        else
            normalized_path += element + "/";
#else
        normalized_path += element + "/";
#endif
    }
    if (!elements.empty() && !ends_in_slash)
        normalized_path = normalized_path.substr(0, normalized_path.length() - 1);

    return location + normalized_path;
}

std::string Path::add_trailing_slash(const std::string &path, PathType path_type)
{
    if (path.empty())
        return path;
    if (path_type == path_type_file) {
#ifdef WIN32
        if (path[path.length() - 1] != '/' && path[path.length() - 1] != '\\')
            return path + "\\";
#else
        if (path[path.length() - 1] != '/' && path[path.length() - 1] != '\\')
            return path + "/";
#endif
        return path;
    } else {
        if (path[path.length() - 1] != '/')
            return path + "/";
        return path;
    }
}

std::string Path::remove_trailing_slash(const std::string &path)
{
    if (path.empty())
        return path;

    if (path[path.length() - 1] == '/' || path[path.length() - 1] == '\\')
        return path.substr(0, path.length() - 1);

    return path;
}

std::string Path::get_location(const std::string &fullname, PathType path_type)
{
#ifdef WIN32
    std::string path = get_fullpath(fullname, path_type);
    if (path_type == path_type_file && path.length() >= 2) {
        if (path[1] == ':')
            return path.substr(0, 2);

        if (path[0] == '\\' && path[1] == '\\') {
            std::string::size_type pos = path.find_first_of("/\\", 2);
            if (pos == std::string::npos)
                return path;
            else
                return path.substr(0, pos);
        }
    }
#endif
    return std::string();
}

std::string Path::get_basepath(const std::string &fullname, PathType path_type)
{
    std::string path = get_fullpath(fullname, path_type);
#ifdef WIN32
    if (path_type == path_type_file && path.length() >= 2) {
        if (path[1] == ':')
            return path.substr(2);

        if (path[0] == '\\' && path[1] == '\\') {
            std::string::size_type pos = path.find_first_of("/\\", 2);
            if (pos == std::string::npos)
                return std::string();
            else
                return path.substr(pos);
        }
    }
#endif
    return path;
}

std::vector<std::string> Path::split_basepath(const std::string &fullname, PathType path_type)
{
    std::string              basepath = get_basepath(fullname, path_type);
    std::vector<std::string> result;
    if (path_type == path_type_file) {
        std::string::size_type start_pos = 0, end_pos = 0;
        while (true) {
            end_pos = basepath.find_first_of("/\\", start_pos);

            if (end_pos == std::string::npos) {
                if (start_pos != basepath.length())
                    result.push_back(basepath.substr(start_pos));
                break;
            }

            result.push_back(basepath.substr(start_pos, end_pos - start_pos));
            start_pos = end_pos + 1;
        }
    } else {
        std::string::size_type start_pos = 0, end_pos = 0;
        while (true) {
            end_pos = basepath.find_first_of('/', start_pos);
            if (end_pos == std::string::npos) {
                if (start_pos != basepath.length())
                    result.push_back(basepath.substr(start_pos));
                break;
            }

            result.push_back(basepath.substr(start_pos, end_pos - start_pos));
            start_pos = end_pos + 1;
        }
    }
    return result;
}

std::string Path::get_fullpath(const std::string &filename, PathType path_type)
{
    if (path_type == path_type_file) {

        std::string::size_type pos = filename.find_last_of("/\\");
        if (pos == std::string::npos) {
#ifdef WIN32
            if (filename.length() >= 2 && filename[1] == ':')
                return filename.substr(0, 2);
#endif
            return std::string();
        }
        return filename.substr(0, pos + 1);
    } else {
        std::string::size_type pos = filename.find_last_of('/');
        if (pos == std::string::npos)
            return std::string();
        return filename.substr(0, pos + 1);
    }
}

std::string Path::get_filename(const std::string &fullname, PathType path_type)
{
    if (path_type == path_type_file) {
        std::string::size_type pos = fullname.find_last_of("/\\");
        if (pos == std::string::npos) {
#ifdef WIN32
            if (fullname.length() >= 2 && fullname[1] == ':')
                return fullname.substr(2);
#endif
            return fullname;
        }
        return fullname.substr(pos + 1);
    } else {
        std::string::size_type pos = fullname.find_last_of('/');
        if (pos == std::string::npos)
            return fullname;
        return fullname.substr(pos + 1);
    }
}

std::string Path::get_basename(const std::string &fullname, PathType path_type)
{
    std::string            filename = get_filename(fullname, path_type);
    std::string::size_type pos      = filename.find_last_of('.');
    if (pos == std::string::npos)
        return filename;
#ifndef WIN32
    // Files beginning with a dot is not a filename extension in Unix.
    // This is different from Windows where it is considered the extension.
    if (path_type == path_type_file && pos == 0)
        return filename;
#endif
    if (path_type == path_type_virtual && pos == 0)
        return filename;
    return filename.substr(0, pos);
}

std::string Path::get_extension(const std::string &fullname, PathType path_type)
{
    std::string            filename = get_filename(fullname, path_type);
    std::string::size_type pos      = filename.find_last_of('.');
    if (pos == std::string::npos)
        return std::string();
#ifndef WIN32
    // Files beginning with a dot is not a filename extension in Unix.
    // This is different from Windows where it is considered the extension.
    if (path_type == path_type_file && pos == 0)
        return std::string();
#endif
    if (path_type == path_type_virtual && pos == 0)
        return std::string();
    return filename.substr(pos + 1);
}

std::string Path::get_fullname(const std::string &fullpath, const std::string &filename, PathType path_type)
{
    return add_trailing_slash(fullpath, path_type) + filename;
}

std::string Path::get_fullname(const std::string &fullpath, const std::string &filename,
                                   const std::string &extension, PathType path_type)
{
    if (!extension.empty() && extension[0] == '.')
        return add_trailing_slash(fullpath, path_type) + filename + extension;
    else if (!extension.empty())
        return add_trailing_slash(fullpath, path_type) + filename + "." + extension;
    else
        return add_trailing_slash(fullpath, path_type) + filename;
}

std::string Path::get_fullname(const std::string &location, const std::string &basepath,
                                   const std::string &filename, const std::string &extension, PathType path_type)
{
    std::string fullname = location;
    fullname += add_trailing_slash(basepath, path_type);
    fullname += filename;
    if (!extension.empty() && extension[0] == '.')
        fullname += extension;
    else if (!extension.empty())
        fullname += "." + extension;
    return fullname;
}

std::string Path::get_realpath(const std::string &path)
{
    return make_absolute("", path);
}

std::string Path::combine(const std::string &part1, const std::string &part2, PathType path_type)
{
    if (!part1.empty())
        return add_trailing_slash(part1, path_type) + part2;
    else
        return part1+part2;
}
} // namespace bv
