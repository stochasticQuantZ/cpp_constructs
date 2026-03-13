#include <iostream>
#include <vector>
#include "linear_search/linear_search.hpp"


bool linear_search(std::vector<int> vec, int element) {
    for (auto &i:vec){
        if( i == element)
            return true;
    }
    return false;
}