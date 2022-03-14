#pragma once
#include <mutex>

template<class T>
class Common {
    public:
    Common(T& other) : obj(other) {}
    T& get() {
        return obj;
    }
    void lock() {
        resource.lock();
    }
    void unlock() {
        resource.unlock();
    }
    private:
    T& obj;
    std::mutex resource;
};
