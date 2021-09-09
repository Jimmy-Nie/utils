/***********************************************************************
 * Description: optional, if cpp is below standard 17 or, contains the optional
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/27.
 * Modify:
 **********************************************************************/

#include "optional/optional.h"

#include <string>
#include <iostream>
#include <utility>

class Person {
public:
    std::optional<std::string> first;
    std::optional<std::string> middle;
    std::optional<std::string> last;

    std::optional<int> age;
    std::optional<std::string> address;
public:
    Person() {}
    Person( std::optional<std::string> f,
            std::optional<std::string> m,
            std::optional<std::string> l,
            std::optional<int> age,
            std::optional<std::string> address)
        :first(std::move(f)), middle(std::move(m)), last(std::move(l)),
        age(std::move(age)), address(std::move(address))
            {}

    void set_name(
            std::optional<std::string> f,
            std::optional<std::string> m,
            std::optional<std::string> l) {
        first = std::move(f);
        middle = std::move(m);
        last = std::move(l);
    }

    friend std::ostream& operator << (std::ostream& strm, const Person& n)
    {
        if (n.first.has_value())
          strm << n.first.value() << ' ';
        if (n.middle)
          strm << n.middle.value() << ' ';
        if (n.last)
          strm << n.last.value();

        if(n.address.has_value())
          strm << ", Address: " <<n.address.value();
        if(n.age.has_value())
           strm << ", Age: " << n.age.value();
        return strm;
    }
};

void prt(Person p) {
    std::cout << p << '\n';
}

void prt_ref(const Person& p) {
    std::cout << p << '\n';
}

void prt_ptr(Person* p) {
    std::cout << *p << '\n';
}

int main() {
    Person n{"Jimmy", std::nullopt, "Nie", std::nullopt, "Pudong"};
    prt(n);
    prt_ref(n);
    prt_ptr(&n);

    Person m{"Bai", "Xiao", "Dong", 22, "Shanghai Pudong"};
    prt(m);
    prt_ref(m);
    prt_ptr(&m);

    Person p;
    p.age = 32;
    p.last = "Hu zhi hao";
    prt(p);
    prt_ref(p);
    prt_ptr(&p);

    return 0;
}