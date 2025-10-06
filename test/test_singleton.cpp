#include "singleton/singleton.hpp"
#include <cassert>
#include <iostream>

//using namespace cpp_constructs;

int main() {
    auto& s1 = cpp_constructs::Singleton::getInstance();
    auto& s2 = cpp_constructs::Singleton::getInstance();

    s1.doSomething();
    // Ensure singleton property
    assert(&s1 == &s2);

    // Test setting and getting
    s1.set("env", "production");
    assert(s2.get("env") == "production");

    std::cout << "All Singleton tests passed!" << std::endl;

    auto x = cpp_constructs::test_le_be();
    return 0;
}

