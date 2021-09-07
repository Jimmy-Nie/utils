#include <iostream>
#include "gpio/gpio.h"
#include <thread>

using namespace jm;

void test_gpio() {
    const int port_num = 36;
    int dir = OUTPUT;
    int value = HIGH;
    GPIO g(port_num);

    for(int i=0; i<10; i++) {
        g.set_dir(dir);
        g.set_value(value);
        std::cout << "port_num:" << g.get_port() << ", dir:"<<g.get_dir() << ", value:" << g.get_value() << std::endl;
        dir = !dir;
        value = !value;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    test_gpio();
    return 0;
}