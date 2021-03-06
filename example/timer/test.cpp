#include "timer/timer.h"
#include "spdlog_impl.h"
#include "timer/datetime.h"

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

void test_datetime() {
    ///1. get utc time
    auto tm = DateTime::get_current_utc_time();
    test_logi("current utc time: {}", tm.to_string());
    test_logi("The timezone:{}", tm.get_timezone());

    ///2. get local time
    tm = DateTime::get_current_local_time();
    test_logi("current local time: {}", tm.to_string());
    test_logi("The timezone:{}", tm.get_timezone());
    test_logi("The date time:{}", tm.to_short_datetime_string());

    ///3. get year month day hour min second
    test_logi("The time: {:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
            tm.get_year(), tm.get_month(), tm.get_day(), tm.get_hour(), tm.get_minutes(), tm.get_seconds());
}

int main() {
    JMLog::init("test");

    test_simple_time();

    test_timer();

    test_datetime();
    return 0;
}