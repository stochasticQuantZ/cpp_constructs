#include "singleton/singleton.hpp"
#include <spdlog/spdlog.h>
namespace cpp_constructs {

Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mtx;

Singleton& Singleton::getInstance() {
    if (!instance) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance) {
            instance = new Singleton();
        }
    }
    return *instance;
}

void Singleton::set(const std::string& key, const std::string& value) {
    store[key] = value;
}

std::string Singleton::get(const std::string& key) const {
    auto it = store.find(key);
    if (it != store.end()) {
        return it->second;
    }
    return {};
}



void Singleton::doSomething(){
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Debugging using spdlog ");
}
} // namespace cpp_constructs

