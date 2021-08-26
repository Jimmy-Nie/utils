/***********************************************************************
 * Description: Thread test
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/4.
 * Modify:
 **********************************************************************/
#include "thread_pool/SingletonThreadPool.h"

#include <iostream>
#include <chrono>

using namespace jm;

auto sleep_ms = [](int ms) {std::this_thread::sleep_for(std::chrono::milliseconds(ms));};

class Temp {
public:
    void foo_member() {
        int i = 0;
        while (++i < 10) {
            std::cout << std::dec << __FUNCTION__ << ": i=" << i << std::endl;
            sleep_ms(200);
        }

        std::cout << std::hex << "Exit thread_pool: " << std::this_thread::get_id() << std::endl;
    }

    void foo_member_with_parm(int n) {
        int i = 0;
        while (++i < n) {
            std::cout << __FUNCTION__ << ": i=" << i << std::endl;
            sleep_ms(200);
        }

        std::cout << std::hex << "Exit thread_pool: " << std::this_thread::get_id() << std::endl;
    }

    static void foo_member_static(int n) {
        int i = 0;
        while (++i < n) {
            std::cout << std::dec << __FUNCTION__ << ": i=" << i << std::endl;
            sleep_ms(200);
        }

        std::cout << std::hex << "Exit thread_pool: " << std::this_thread::get_id() << std::endl;
    }
};

void foo_normal() {
    int i = 0;
    while (++i < 10) {
        std::cout << std::dec << __FUNCTION__ << ": i=" << i << std::endl;
        sleep_ms(200);
    }

    std::cout << std::hex << "Exit thread_pool: " << std::this_thread::get_id() << std::endl;
}

void test_thread_pool() {
    auto tp = SingletonThreadPool::getInstance();

    ///1. Normal function
    tp->enqueue(foo_normal);

    ///2. class member function
    Temp tmp;
    sleep_ms(2);
    tp->enqueue(&Temp::foo_member, tmp);
    sleep_ms(2);
    tp->enqueue(&Temp::foo_member_with_parm, tmp, 10);
    sleep_ms(2);
    tp->enqueue(&Temp::foo_member_static, 10);

    ///3. stop
    tp->stop();
    try {
        for(int i=10; i<100; i++)
            tp->enqueue(foo_normal);
    } catch (std::exception& ec) {
        std::cout << "Exception: " << ec.what() << std::endl;
    }
}


int main() {
    test_thread_pool();


}