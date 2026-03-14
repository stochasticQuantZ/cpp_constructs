#pragma once

#include <string>
#include <vector>

inline void generate_combinations_recursive(
    const std::string& input,
    std::size_t index,
    std::string current,
    std::vector<std::string>& result)
{
    if (index == input.size()) {
        if (!current.empty()) {
            result.push_back(current);
        }
        return;
    }

    // Exclude current character
    generate_combinations_recursive(input, index + 1, current, result);

    // Include current character
    current.push_back(input[index]);
    generate_combinations_recursive(input, index + 1, current, result);
}

inline std::vector<std::string> get_all_combinations(const std::string& input)
{
    std::vector<std::string> result;
    std::string current;
    generate_combinations_recursive(input, 0, current, result);
    return result;
}