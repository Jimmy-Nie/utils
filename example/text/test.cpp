#include "text/strings.h"
#include "text/file.h"
#include "spdlog_impl.h"
#include <iostream>
using namespace jm;

void test_string() {
    test_logi("Trim: {}", Strings::trim("\r\nThis is a word!\r\n"));
    test_logi("get file name: {}", Strings::get_file_name("/home/user/a.txt"));

    std::string str_double = "3.14";
    auto i = Strings::string_to<int>(str_double);
    if(i)
        test_logi("The value: {}", i.value());

    std::string str_bool = "True";
    i = Strings::string_to_bool(str_bool);
    if(i)
        test_logi("The value: {}", i.value());
}

void test_file() {
    std::string file_name = "./test.txt";
    std::string content = "Hello, this is only test !!!!";
    File ifile(file_name);
    File::write_text(file_name, content);
    test_logi("The content: {}", File::read_text(file_name));
}

int main() {
    JMLog::init();
    
    test_string();
    test_file();
    
    return 0;
}