#pragma once
#include <string>

namespace jm
{

enum INTERNAL_STATE {
    LOW = 0,
    HIGH = 1,
    OUTPUT = 0,
    INPUT = 1
};

class GPIO {
public:
    /// \brief: construct the GPIO
    /// \param: port_num - the gpio port_num
    /// dir - input/output
    /// val - low/high
    GPIO();
    GPIO(int port_num);
    GPIO(const std::string& port_num);
    GPIO(int port_num, int dir, int val);
    GPIO(const std::string& port_num, const std::string& dir, const std::string& val);

    /// \brief: set/get the gpio port, directory, value and all of the arguments
    int get_port();
    void set_port(int port_num);
    void set_port(const std::string& port_num);

    int get_dir();
    int set_dir(int dir);
    int set_dir(const std::string& dir);

    int get_value();
    int set_value(int val);
    int set_value(const std::string& val);

    int set_gpio(int port_num, int dir, int val);
    int set_gpio(const std::string& port_num, const std::string& dir, const std::string& val);

    /// \brief: export/unexport the gpio port
    int export_gpio();
    int unexport_gpio();

    int close();
private:
    std::string port_num_;
};

}
