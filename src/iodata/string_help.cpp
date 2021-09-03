
#include <algorithm>
#include <cstring>
#include <map>

#include "string_help.h"

namespace jm {

std::map<std::string, std::string> StringHelp::split_by_delimiter(const std::string &src, const std::string &delimiter) {
    std::map<std::string, std::string> str_map;

    /// 1. Check the map string
    if (src.empty() || delimiter.empty()) {
        return str_map;
    }

    /// 2. split the string
    auto pos = src.find(delimiter);
    if (pos == std::string::npos) { // did not found the delimiter
        return str_map;
    }
    /// 3. Get rid of the space and tab before and after
    auto first_str = trim(src.substr(0, pos));
    auto second_str = trim(src.substr(pos + delimiter.length()));

    /// 4. insert the pair to map
    if (!first_str.empty() && !second_str.empty())
        str_map.insert(std::pair<std::string, std::string>(first_str, second_str));

    return str_map;
}

std::vector<std::string> StringHelp::split_text(const std::string &text, const std::string &split_string,
                                                bool skip_empty) {
    std::vector<std::string> result;
    std::string::size_type end_pos = 0, begin_pos = 0;
    while (true) {
        end_pos = text.find(split_string, begin_pos);
        if (end_pos == std::string::npos) {
            if (begin_pos != text.length())
                result.push_back(text.substr(begin_pos));
            break;
        } else {
            if (!skip_empty || begin_pos != end_pos)
                result.push_back(text.substr(begin_pos, end_pos - begin_pos));
            begin_pos = end_pos + split_string.length();
        }
    }
    return result;
}

std::string StringHelp::get_file_name(const std::string &text, bool suffix) {
    std::string::size_type first_char = text.find_last_of("/\\");
    if (first_char == std::string::npos)
        return text;

    std::string file_name = text.substr(first_char + 1, text.size() - first_char);
    if (suffix) {
        return file_name;
    }

    std::string::size_type last_char = file_name.find_last_of('.');
    if (last_char == std::string::npos)
        return file_name;
    return file_name.substr(0, last_char);
}

std::string StringHelp::replace_string(const std::string &text, const std::string &sub, const std::string &after) {
    int p = 0;
    std::string ret = text;
    while ((p = ret.find(sub, p)) != std::string::npos) {
        ret.replace(p, sub.size(), after);
        p += after.size();
    }
    return ret;
}

std::string StringHelp::trim(const std::string &text) {
    std::string::size_type first_char = text.find_first_not_of(" \r\n\t");
    std::string::size_type last_char = text.find_last_not_of(" \r\n\t");
    if (first_char == std::string::npos)
        return std::string();
    if (last_char == std::string::npos)
        return std::string();
    return text.substr(first_char, last_char - first_char + 1);
}

int StringHelp::compare(const std::string &a, const std::string &b, bool case_sensitive) {
#ifdef WIN32
    if (case_sensitive)
        return strcmp(a.c_str(), b.c_str());
    else
        return stricmp(a.c_str(), b.c_str());
#else
    if (case_sensitive)
        return strcmp(a.c_str(), b.c_str());
    else
        return strcasecmp(a.c_str(), b.c_str());
#endif
}

std::string StringHelp::string_to_upper(const std::string &s) {
    std::string result = s;
    std::string::size_type index, size;
    size = result.length();
    for (index = 0; index < size; index++) {
        result[index] = (unsigned char) toupper((unsigned char) result[index]);
    }
    return result;
}

std::string StringHelp::string_to_lower(const std::string &s) {
    std::string result = s;
    std::string::size_type index, size;
    size = result.length();
    for (index = 0; index < size; index++) {
        result[index] = (unsigned char) tolower((unsigned char) result[index]);
    }
    return result;
}

std::string StringHelp::remove_trailing_zeros(std::string text) {
    if (text.find_first_of('.') != std::string::npos) {
        while (!text.empty() && text.back() == '0')
            text.pop_back();
        if (!text.empty() && text.back() == '.')
            text.pop_back();
    }
    return text;
}

std::optional<long double> StringHelp::string_to_ld(const std::string &value) {
    try {
        return std::stold(value);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<unsigned long long> StringHelp::string_to_ull(const std::string &value, int base) {
    try {
        return std::stoull(value, nullptr, base);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<long long> StringHelp::string_to_ll(const std::string &value, int base) {
    try {
        return std::stoll(value, nullptr, base);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<bool> StringHelp::string_to_bool(const std::string &value) {
    auto is_str = [&](const std::string &str) { return StringHelp::compare(value, str, false) == 0; };
    return is_str("true") || is_str("yes") || is_str("on") || value == "1";
}

std::string StringHelp::left(const std::string &s, size_t n) {
    if (n >= s.size()) {
        return s;
    }
    return s.substr(0, n);
}

std::string StringHelp::right(const std::string &s, size_t n) {
    if (n >= s.size()) {
        return s;
    }
    return s.substr(s.size() - n);
}

bool StringHelp::start_with(const std::string &a, const std::string &b) {
    if (b.empty() && !a.empty()) {
        return false;
    }
    return left(a, b.size()) == b;
}

bool StringHelp::end_with(const std::string &a, const std::string &b) {
    if (b.empty() && !a.empty()) {
        return false;
    }
    return right(a, b.size()) == b;
}

}