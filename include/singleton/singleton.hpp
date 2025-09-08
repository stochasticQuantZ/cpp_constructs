#pragma once
#include <string>
#include <mutex>
#include <unordered_map>

namespace cpp_constructs {

class Singleton {
public:
    // Get the singleton instance
    static Singleton& getInstance();

    // Delete copy constructor & assignment
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Example methods
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;
    void doSomething();    

private:
    Singleton() = default;   // private constructor
    ~Singleton() = default;  // private destructor

    static Singleton* instance;
    static std::mutex mtx;

    std::unordered_map<std::string, std::string> store;
};

} // namespace cpp_constructs

