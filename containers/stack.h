#pragma once

#include <cassert>
#include <mutex>
#include <memory>

template<class T>
class Stack {
public:
    Stack(std::size_t _size) : size(_size), count(0) {
        data = new T[size];
    }
    void push(const T& val) {
        std::lock_guard<std::mutex> guard(resource);
        assert(count < size);
        data[count] = val;
        count++;
    }
    void pop() {
        std::lock_guard<std::mutex> guard(resource);
        assert(count > 0);
        count--;
    }
    T& top() {
        std::lock_guard<std::mutex> guard(resource);
        assert(count > 0);
        return data[count - 1];
    }
    ~Stack() {
        delete[] data;
        data = nullptr;
    }
private:
    std::mutex resource;
    T* data;
    std::size_t size;
    std::size_t count;
};