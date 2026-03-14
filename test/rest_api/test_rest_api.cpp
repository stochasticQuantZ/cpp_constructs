#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(RestApiJsonTest, ParseSampleResponse) {
    const std::string sample = R"({
        "userId": 1,
        "id": 1,
        "title": "delectus aut autem",
        "completed": false
    })";

    json body = json::parse(sample);

    EXPECT_EQ(body["userId"], 1);
    EXPECT_EQ(body["id"], 1);
    EXPECT_EQ(body["title"], "delectus aut autem");
    EXPECT_EQ(body["completed"], false);
}