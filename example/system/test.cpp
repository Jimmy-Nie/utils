//
// Created by Administrator on 2021/8/31.
//

#include "system/system.h"
#include "spdlog_impl.h"

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

int main() {
    JMLog::init("test");

    test_system();
    return 0;
}