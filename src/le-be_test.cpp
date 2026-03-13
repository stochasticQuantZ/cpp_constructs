#include <iostream>
#include "singleton/singleton.hpp"
#include <nlohmann/json.hpp>

#include <fmt/format.h>

namespace cpp_constructs {


int test_le_be(void) {
    unsigned int i = 1;
    char *c = (char *) &i; // Treat the integer as a sequence of bytes

    if (*c) { // Check the first byte
        std::cout << "System is Little-Endian\n" << std::endl;
    } else {
        std::cout << "System is Big-Endian\n" << std::endl;
    }

    std::cout << "JSON _version" << NLOHMANN_JSON_VERSION_MAJOR << "." << NLOHMANN_JSON_VERSION_MINOR << "." << NLOHMANN_JSON_VERSION_PATCH << std::endl;
    std::cout << " FMT Version: " << FMT_VERSION << std::endl;
 
    return 0;
}

}