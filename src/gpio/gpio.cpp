#include "gpio.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
namespace jm
{

GPIO::GPIO() {}

GPIO::GPIO(const string& port_num) {
    port_num_ = port_num;
}

GPIO::GPIO(int port_num) {
    port_num_ = to_string(port_num);
}

GPIO::GPIO(const std::string& port_num, const std::string& dir, const std::string& val) {
    set_gpio(port_num, dir, val);
}

GPIO::GPIO(int port_num, int dir, int val) {
    set_gpio(port_num, dir, val);
}


int GPIO::get_port() {
    return std::stoi(port_num_);
}

void GPIO::set_port(const string& port_num) {
    port_num_ = port_num;
}

void GPIO::set_port(int port_num) {
    port_num_ = to_string(port_num);
}


int GPIO::get_dir() {
    string   setdir_str = "/sys/class/gpio/gpio" + port_num_ + "/direction";
    ifstream setdirgpio(setdir_str.c_str());
    if (!setdirgpio.is_open())
        return false;

    std::string dir_str;
    setdirgpio >> dir_str;
    setdirgpio.close();

    int dir = OUTPUT;
    if(dir_str=="in")
        dir = INPUT;

    return dir;
}

int GPIO::set_dir(int dir) {
    string dir_str = (dir)? "int" : "out";
    return set_dir(dir_str);
}

int GPIO::set_dir(const string& dir) {
    string   setdir_str = "/sys/class/gpio/gpio" + port_num_ + "/direction";
    ofstream setdirgpio(setdir_str.c_str());
    if (!setdirgpio.is_open())
        return false;

    setdirgpio << dir;
    setdirgpio.close();
    return true;
}

int GPIO::get_value() {
    int      val;
    string   getval_str = "/sys/class/gpio/gpio" + port_num_ + "/value";
    ifstream getvalgpio(getval_str.c_str());
    if (!getvalgpio.is_open())
        return false;
    
    getvalgpio >> val;
    getvalgpio.close();

    return static_cast<bool>(val);
}

int GPIO::set_value(const string& val) {
    string   setval_str = "/sys/class/gpio/gpio" + port_num_ + "/value";
    ofstream setvalgpio(setval_str.c_str());
    if (!setvalgpio.is_open())
        return false;;

    setvalgpio << val;
    setvalgpio.close();
    return true;
}

int GPIO::set_value(int val) {
    return set_value(std::to_string(val));
}


int GPIO::set_gpio(const std::string& port_num, const std::string& dir, const std::string& val) {
    bool ret = true;

    ret &= export_gpio();
    set_port(port_num);
    ret &= set_dir(dir);
    ret &= set_value(val);

    return ret;
}

int GPIO::set_gpio(int port_num, int dir, int val) {
    bool ret = true;

    ret &= export_gpio();
    set_port(port_num);
    ret &= set_dir(dir);
    ret &= set_value(val);

    return ret;
}


int GPIO::export_gpio() {
    string   export_str = "/sys/class/gpio/export";
    ofstream exportgpio(export_str.c_str());
    if (!exportgpio.is_open())
        return false;

    exportgpio << port_num_;
    exportgpio.close();
    return true;
}

int GPIO::unexport_gpio() {
    string   unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str());
    if (!unexportgpio.is_open())
        return false;

    unexportgpio << port_num_;
    unexportgpio.close();
    return true;
}

int GPIO::close() {
    return unexport_gpio();
}

}