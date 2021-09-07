///\brief: this list the most basic usage
///     if you need advanced usage, pls visit github:https://github.com/nlohmann/json

#include "json/json.h"
#include <iostream>

using nlohmann::json;
void test_json() {
    /// create an empty structure (null)
    json j1;
    j1["pi"] = 3.141;
    j1["happy"] = true;
    j1["name"] = "Niels";
    j1["nothing"] = nullptr;    // add another null object by passing nullptr
    j1["answer"]["everything"] = 42;// add an object inside the object
    j1["list"] = { 1, 0, 2 };    // add an array that is stored as std::vector (using an initializer list)
    j1["object"] = { {"currency", "USD"}, {"value", 42.99} };// add another object (using an initializer list of pairs)

    /// instead, you could also write (which looks very similar to the JSON above)
    json j2 = {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {
                           {"everything", 42}
                   }},
            {"list", {1, 0, 2}},
            {"object", {
                           {"currency", "USD"},
                         {"value", 42.99}
                   }}
    };

    /// create object from string literal
    json j3 = "{ \"happy\": true, \"pi\": 3.141 }"_json;

    /// parse explicitly
    auto j4 = json::parse(R"({"happy": true, "pi": 3.141})");

    std::cout << j1.dump(4) << std::endl;
    std::cout << j2.dump(4) << std::endl;
    std::cout << j3.dump(4) << std::endl;
    std::cout << j4.dump(4) << std::endl;
}

int main() {
    test_json();
    return 0;
}