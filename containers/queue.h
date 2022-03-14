#pragma once

#include <cassert>
#include <mutex>
#include <memory>

template<class T>
class Queue {
public:
    Queue(std::size_t _size) : size(_size) {
        first = last = count = 0;
        data = new T[size];
    }
    void push(const T& val) {
        std::lock_guard<std::mutex> guard(resource);
        if (count == size)
            first = (first + 1) % size;
        data[last] = val;
        last = (last + 1) % size;
        count = std::min(size, count + 1);
    }
    T& front() {
        std::lock_guard<std::mutex> guard(resource);
        assert(count > 0);
        return data[first];
    }
    void pop() {
        std::lock_guard<std::mutex> guard(resource);
        assert(count > 0);
        count--;
        first = (first + 1) % size;
    }
    ~Queue() {
        delete[] data;
        data = nullptr;
    }

private:
    std::mutex resource;
    T* data;
    std::size_t size;
    std::size_t first;
    std::size_t last;
    std::size_t count;
};