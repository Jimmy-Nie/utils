#include "iodata/string_help.h"
#include "iodata/databuffer.h"
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
    ///1. Data buffer store the string
    DataBuffer buf(256);
    int len = 0;
    for(int i=0; i<100; i++) {
        std::string str = std::to_string(i) + " ";
        buf.set_size(len+str.length()); ///in case that buf data out of range
        memcpy(buf.get_data()+len, str.c_str(), str.length());
        len += str.length();
    }
    test_logi("The size={}, capacity={}, data: {}", buf.get_size(), buf.get_capacity(), buf.get_data());

    ///2. Data buffer store the hex, and get the data by type
    DataBuffer buf2(256);
    for(int i=0; i<4; i++) {
        buf2[i] = i;
        std::cout <<  (buf[i]) << std::endl;
    }

     std::cout << std::hex << *(buf.get_data<int>()) << std::endl;
}

int main() {
    JMLog::init();
    test_string();
    test_databuf();

    return 0;
}