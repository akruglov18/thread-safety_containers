#pragma once
#include <mutex>

template<class T>
class Common {
    public:
    Common(T& other) : obj(other) {}
    T& get() {
        return obj;
    }
    std::mutex& get_mutex() {
        return resource;
    }
    private:
    T& obj;
    std::mutex resource;
};
