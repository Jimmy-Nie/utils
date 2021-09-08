#include <iostream>
#include "event/event_dispatcher.h"

using namespace jm;

///=================================================================
///Simple test
enum Color {
    kRed,
    kYellow,
    kGreen
};

EventDispatcher<int, void()> ed;

void red_light() {
    std::cout << "Red light \n";
}

void yellow_light() {
    std::cout << "Yellow light \n";
}

void green_light() {
    std::cout << "Green light \n";
}

void test_simple_event() {
    ed.appendListener(kRed, std::bind(red_light));
    ed.appendListener(kYellow, std::bind(yellow_light));
    ed.appendListener(kGreen, std::bind(green_light));

    for(int i=kRed; i<=kGreen; i++) {
        ed.dispatch(i);
    }
}

///======================================================================
///Event class
class EventTest {
public:
    EventTest() {
        log_event_.appendListener("Login", std::bind(&EventTest::log_in, this, std::placeholders::_1));
        log_event_.appendListener("Logout", std::bind(&EventTest::log_out, this, std::placeholders::_1));
    }

    void on_event_received(const Event& e) {
        log_event_.dispatch(e.get_name(), e);
    }

private:
    void log_in(const Event& e) {
        login_flag_ = true;
        std::cout << "Event: " << e.get_name() << std::endl;
        std::cout << "Args: " << e.to_string() << std::endl;
        std::cout << std::endl;
    }

    void log_out(const Event& e) {
        login_flag_ = false;
        std::cout << "Event: " << e.get_name() << std::endl;
        std::cout << "Args: " << e.to_string() << std::endl;
        std::cout << std::endl;
    }

private:
    bool login_flag_ = false;
    EventDispatcher<std::string, void(Event)> log_event_;
};


int main() {
    test_simple_event();

    EventTest tst;
    tst.on_event_received(Event("Login", {1,2, "log_in"}));
    tst.on_event_received(Event("Logout", {-1,-2, "log_out"}));

    return 0;
}