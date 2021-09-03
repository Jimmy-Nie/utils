#include "iodata/string_help.h"
#include "spdlog_impl.h"
#include <iostream>
using namespace jm;

void test_string() {
    test_logi("Trim: {}", StringHelp::trim("\r\nThis is a word!\r\n"));
    test_logi("get file name: {}", StringHelp::get_file_name("/home/user/a.txt"));

    std::string str_double = "3.14";
    auto i = StringHelp::string_to<int>(str_double);
    if(i)
        test_logi("The value: {}", i.value());

    std::string str_bool = "True";
    i = StringHelp::string_to_bool(str_bool);
    if(i)
        test_logi("The value: {}", i.value());
}

void test_databuf() {

}

int main() {
    JMLog::init();
    test_string();
    test_databuf();

    return 0;
}