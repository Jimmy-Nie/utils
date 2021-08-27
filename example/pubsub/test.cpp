/***********************************************************************
 * Description: public and subscribe test
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/27.
 * Modify:
 **********************************************************************/
#include "pubsub/pubsub.h"
#include "spdlog_impl.h"

#include <string>
#include <thread>
#include <chrono>
#include <functional>

using namespace jm;

auto sleep_ms = [](int ms) {std::this_thread::sleep_for(std::chrono::milliseconds(ms));};

struct message {
    int id_ = 0;
    std::string data_;
    
    message(int id, const std::string& data) : id_(id), data_(data) {}
};

void pub_th() {
    int i = 0;
    while(++i < 100) {
        message msg(i, "hello, world: " + std::to_string(i));
        publish<message>("test", msg);
        sleep_ms(1000);
        jm_logi("publish message times:{}", i);
    }
    jm_logw("Thread exit ...");//, std::this_thread::get_id());
}


void prt_normal(const message& msg) {
    jm_logi("nromal function: id={}, str={}", msg.id_, msg.data_);
}

class Tst {
public:
    void prt_member(const message& msg) {
        jm_logi("member function: id={}, str={}", msg.id_, msg.data_);
        sleep_ms(1000);
    }
};

int main() {
    JMLog::init("jm");

    std::thread pub(&pub_th);

    Subscriber<message> sb1("test", std::bind(&prt_normal, std::placeholders::_1));
    sb1.subscribe();

    Tst tst;
    Subscriber<message> sb2("test", std::bind(&Tst::prt_member, tst, std::placeholders::_1));
    sb2.subscribe();

    pub.join();

    return 0;
}