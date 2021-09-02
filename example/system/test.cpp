//
// Created by Administrator on 2021/8/31.
//

#include "system/system.h"
#include "system/endian.h"
#include "spdlog_impl.h"
#include <iostream>

using namespace jm;

void test_system() {
    ///1. get cpu cores
    test_logi("CPU cores: {}", System::get_num_cores());
    test_logi("Execute path: {}", System::get_exe_path());
    test_logi("Timer: {}" , System::get_time());

    std::string cmd = "cat /proc/cpuinfo";
    test_logi("system call: \n{}", System::system_call(cmd));

    System::system_call_async(cmd, std::bind([](bool result){test_logi("System call result: {}", result);}, std::placeholders::_1));
    System::sleep(10);
}

void test_endian() {
    test_logi("This system is {} endian", Endian::is_system_big()? "big" : "little");
    test_logi("This CPU {} bits", Endian::is_system_64bit()? "64" : "32");

    int i = 0x12345678;
    int j = 0x12345678;
    char k[] = "hello, world!";

    Endian::swap_to_big_endian(&i, sizeof(i));
    Endian::swap_to_little_endian(&j, sizeof(j));
    Endian::swap_to_big_endian(k, sizeof(k)-1); ///must size -1, for string end by "\0"

    test_logi("Big endian: i=0x{:x} j=0x{:x}, k={}", i, j, k);
}

int main() {
    JMLog::init("test");

    test_system();
    test_endian();
    return 0;
}