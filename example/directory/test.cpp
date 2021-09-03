
#include "directory/path.h"
#include "spdlog_impl.h"
#include "directory/directory.h"
#include "timer/timer.h"

using namespace jm;

void test_path() {
    std::string file_path = "/mnt/UDISK/a.txt";
    test_logi("Base Path: {}", Path::get_basepath(file_path));
    test_logi("Base Name: {}", Path::get_basename(file_path));
    test_logi("Full Path: {}", Path::get_fullpath(file_path));
    test_logi("File extension: {}", Path::get_extension(file_path));

    std::string real_path = "/home/jimmy/a/";
    test_logi("Real Path: {}", Path::get_realpath(real_path));
    test_logi("Combine: {}", Path::combine(real_path, file_path));

}

void test_directory() {
    Directory::create("/mnt/UDISK/");
    Directory::create("./mnt/UDISK/");
    Directory::create("../mnt/UDISK/");

    SimpleTimer::sleep_s(30);

    Directory::remove("/mnt/UDISK/");
    Directory::remove("./mnt/UDISK/");

    test_logi("The current dir: {}", Directory::get_current());
}

int main() {
    JMLog::init();

    test_path();

    test_directory();

    return 0;
}