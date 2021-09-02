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

class Name {
private:
    std::string first;
    std::optional<std::string> middle;
    std::string last;

public:
    Name(std::string f,
         std::optional<std::string> m,
         std::string l)
            : first{ std::move(f) }, middle{ std::move(m) }, last{ std::move(l) }
    {
    }

    friend std::ostream& operator << (std::ostream& strm, const Name& n)
    {
        strm << n.first << ' ';
        if (n.middle) {
            strm << n.middle.value() << ' ';
        }
        return strm << n.last;
    }
};
int main() {
    Name n{"Jimmy", std::nullopt, "Nie"};
    std::cout << n << '\n';
    Name m{"Zhang", "Lao", "San"};
    std::cout << m << '\n';

    return 0;
}