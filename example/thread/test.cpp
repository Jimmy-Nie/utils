/***********************************************************************
 * Description: Thread test
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/4.
 * Modify:
 **********************************************************************/
#include "thread/threads.h"
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
            sleep_ms(100);
            std::cout << "Foo running secs: " << ++i/10.0 << std::endl;
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
            sleep_ms(100);
            std::cout << "Bar running secs: " << ++i/10.0 << std::endl;
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

int main() {
    Foo foo;
    Bar bar;

    foo.start();
    bar.start();

    sleep_ms(5e3);
    foo.stop();
    bar.stop();

    sleep_ms(1e3);
    return 0;
}