#include "enum/enum.h"
#include <iostream>

BETTER_ENUM(Color, uint32_t,
        kRed,
        kYellow,
        kGreen,
        kBlue = 10);

void test_enum() {
    /// You cannot convert a literal constant such as Color::kRed directly to,
    /// for example, a string. You have to prefix it with +:
    std::cout << (+Color::kRed)._to_string() << ", "
            << (+Color::kYellow)._to_string() << "!\n";

    ///_from_string and _to_string
    Color c = Color::kGreen;
    std::cout << c._to_string() << std::endl;

    c = Color::_from_string("kBlue");
    std::cout << c._to_string() << std::endl;

    ///_fro_integral and _to_integral, _to_index
    c = Color::_from_integral(10);
    std::cout << c._to_string() << std::endl;
    std::cout << c._to_integral() << std::endl;
    std::cout << c._to_index() << std::endl;

    auto cl = Color::_from_integral_nothrow(0);
    if(cl)
        std::cout << cl->_to_string() << std::endl;


    ///Better Enums also provides three validity checking functions,
    /// one for each of the groups of conversions - string, case-insensitive string, and integer
    std::cout << std::boolalpha << Color::_is_valid(3) << std::endl;
    std::cout << std::boolalpha << Color::_is_valid("kBlue") << std::endl;
    std::cout << std::boolalpha << Color::_is_valid_nocase("kred") << std::endl;

    /// Better Enums makes it easy to iterate over the values you have declared.
    for(int i=0; i<Color::_size(); i++) {
        std::cout << "value: " << Color::_values()[i]._to_integral();
        std::cout << ", name: " << Color::_names()[i];
        std::cout << std::endl;
    }
}


int main() {
    test_enum();

    return 0;
}