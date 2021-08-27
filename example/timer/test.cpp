#include "timer/timer.h"
#include "spdlog_impl.h"

using namespace jm;

void test_simple_time() {
    SimpleTimer tm;
    tm.sleep_s(2);
    test_logi("Elapsed s: {}", tm.elapsed_s());
    test_logi("Elapsed ms: {}", tm.elapsed_ms());
    test_logi("Elapsed us: {}", tm.elapsed_us());
    test_logi("Elapsed ns: {}", tm.elapsed_ns());

    tm.start();
    tm.sleep_ms(2100);
    test_logi("Elapsed s: {}", tm.elapsed_s());
    test_logi("Elapsed ms: {}", tm.elapsed_ms());
    test_logi("Elapsed us: {}", tm.elapsed_us());
    test_logi("Elapsed ns: {}", tm.elapsed_ns());
}

int main() {
    JMLog::init("test");

    test_simple_time();

    return 0;
}