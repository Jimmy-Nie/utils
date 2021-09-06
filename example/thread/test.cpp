/***********************************************************************
 * Description: Thread test
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/4.
 * Modify:
 **********************************************************************/
#include "thread/threads.h"
#include "thread/singleton_thread_pool.h"
#include <iostream>

using namespace jm;

auto sleep_ms = [](int ms) {std::this_thread::sleep_for(std::chrono::milliseconds(ms));};

class Foo : public Thread {
public:
    Foo() {
        start_con_ = started.connect( this, &Foo::startFunc);
        stop_con_ = finished.connect(this, &Foo::stopFunc);
    }

    void run() {
        int i = 0;
        while(isRunning()) {
            sleep_ms(1000);
            std::cout << "Foo running secs: " << ++i << std::endl;
        }
    }

    void startFunc() {
        std::cout << "Foo starting ..." << std::endl;
    }

    void stopFunc() {
        std::cout << "Foo exit, id=" << getID();
    }

private:
    Connection start_con_;
    Connection stop_con_;
};


class Bar : public Thread {
public:
    Bar() {
        start_con_ = started.connect( this, &Bar::startFunc);
        stop_con_ = finished.connect(this, &Bar::stopFunc);
    }

    void run() override {
        int i = 0;
        while(isRunning()) {
            sleep_ms(1000);
            std::cout << "Bar running secs: " << ++i << std::endl;
        }
    }

    void startFunc() {
        std::cout << "Bar starting ..." << std::endl;
    }

    void stopFunc() {
        std::cout << "Bar exit, id=" << getID();
    }

private:
    Connection start_con_;
    Connection stop_con_;
};

void test_thread() {
    Foo foo;
    //Bar bar;

    foo.start();
    //bar.start();

    foo.wait(5000);
    foo.stop();
    std::cout << "STOP!!!!!!!!"<<std::endl;
    foo.wait();

    //bar.stop();
}

///================================================================================================
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
        std::cout << "exception: " << ec.what() << std::endl;
    }
}


int main() {
    test_thread();
    test_thread_pool();
    return 0;
}