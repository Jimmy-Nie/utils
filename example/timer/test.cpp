#include "timer/timer.h"
#include "spdlog_impl.h"
#include <iostream>

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

void test_timer() {
    Timer tm;
    const uint32_t timeout = 2000;
    int times = 0;
    tm.func_expired() = std::bind([&]() { test_logi("Time out {} secs", (++times) * timeout / 1000);});

    tm.start(timeout);      ///this must after set func_expired, otherwise, expired func is invalid

    SimpleTimer stm;
    stm.sleep_ms(timeout*10);

    tm.stop();
}

int main() {
    JMLog::init("test");

    test_simple_time();

    test_timer();

    return 0;
}