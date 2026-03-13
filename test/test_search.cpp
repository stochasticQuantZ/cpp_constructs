#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <vector>
#include "linear_search/linear_search.hpp"


TEST(LinearSearchTest, Basic) {
    // simple sanity check
    std::vector<int> v{1,2,3,4,5,6,7,8,9,10};

    EXPECT_EQ(linear_search(v,5), true);
    std::cout << "Linearly searching" << std::endl;
    EXPECT_FALSE(linear_search(v, 111));
    EXPECT_EQ(linear_search(v,11), false);

}
