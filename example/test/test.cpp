// Created by Administrator on 2021/9/1.
//

#include <iostream>
#include <chrono>
#include <ratio>
#include <memory>

using namespace std::chrono;

int test_duration() {
    typedef std::chrono::duration<int> seconds_type;
    typedef std::chrono::duration<int,std::milli> milliseconds_type;
    typedef std::chrono::duration<int,std::ratio<60*60>> hours_type;

    hours_type h_oneday (24);                  // 24h
    seconds_type s_oneday (60*60*24);          // 86400s
    milliseconds_type ms_oneday (s_oneday);         // 86400000ms

    seconds_type s_onehour (60*60);            // 3600s
    hours_type h_onehour (std::chrono::duration_cast<hours_type>(s_onehour));
    milliseconds_type ms_onehour (s_onehour);       // 3600000ms (ok, no type truncation)

    std::cout << ms_onehour.count() << "ms in 1h" << std::endl;

    return 0;
}

int test_time_point() {
    time_point<system_clock, duration<int,  std::milli>> tp_sec(duration<int, std::milli>(1));
    system_clock::time_point tp(tp_sec);
    std::cout << "1s since system_clock epoch: " << tp.time_since_epoch().count() << std::endl;

    system_clock::time_point tp_now = system_clock::now();
    system_clock::duration dtn = tp_now.time_since_epoch();
    std::cout << "Duration since epoch: " << dtn.count() << std::endl;
    std::cout << "sec: " << dtn.count()*system_clock::period::num/system_clock::period::den << std::endl;
}

void test_system_clock() {
    system_clock::time_point today = system_clock::now();
    std::chrono::duration<int, std::ratio<60*60*24>> one_day(1);
    //std::chrono::duration<int, std::ratio<60*60*24> > one_day (1);

    system_clock::time_point tomorrow = today + one_day;

    auto td = system_clock::to_time_t(today);
    auto tw = system_clock::to_time_t(tomorrow);
    std::cout << "Today:" << ctime(&td) << ", Tomorrow:" << ctime(&tw) << std::endl;
}

void test_steady_clock() {
    steady_clock::time_point before = steady_clock::now();
    for(int i=0; i<1000; i++)
        std::cout << ".";
    steady_clock::time_point after = steady_clock::now();
    duration<int, std::nano> span = duration_cast<duration<int, std::nano>>(after-before);
    std::cout << "\nEplased: " << span.count() << " ns" << std::endl;
}

void test_system_start_duration() {
    auto stc_now = std::chrono::steady_clock::now();
    auto sys_now = std::chrono::system_clock::now();
    std::cout << "steady_clock since epcho hours: " <<
        stc_now.time_since_epoch().count() * std::chrono::system_clock::period::num/std::chrono::system_clock::period::den/3600.0 << std::endl;

    std::cout << "system_clock since epcho years: " <<
        sys_now.time_since_epoch().count() * std::chrono::system_clock::period::num/std::chrono::system_clock::period::den/(3600*24*365.0) << std::endl;

    std::ratio<10, 1> r10;
    std::cout << " std::ratio<10, 2>::num=" << std::ratio<10, 2>::num << std::endl;
    std::cout << " std::ratio<10, 2>::den=" << std::ratio<10, 2>::den << std::endl;

    std::cout << "std::chrono::system_clock::period::num=" << std::chrono::system_clock::period::num << std::endl;
    std::cout << "std::chrono::system_clock::period::den=" << std::chrono::system_clock::period::den << std::endl;
}

int main() {
/*    test_duration();
    test_time_point();
    test_system_clock();
    test_steady_clock();*/
    test_system_start_duration();

}