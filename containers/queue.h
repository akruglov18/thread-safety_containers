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
    Queue(const Queue& other) : size(other.size) {
        first = last = count = 0;
        data = new T[size];
        for (std::size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    void push(const T& val) {
        std::lock_guard<std::mutex> guard(resource);
        if (count == size)
            first = (first + 1) % size;
        data[last] = val;
        last = (last + 1) % size;
        count = std::min(size, count + 1);
    }
    T front() {
        std::lock_guard<std::mutex> guard(resource);
        assert(count > 0);
        T temp = data[first];
        count--;
        first = (first + 1) % size;
        return temp;
    }

    std::size_t get_size() const {
        return count;
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