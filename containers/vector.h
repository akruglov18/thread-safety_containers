#pragma once

#include "utils.h"
#include <iostream>
#include <mutex>
#include <cassert>
#include <atomic>

template<class T>
class SynchVector {
public:
    SynchVector() : capacity(0), size(0), data(nullptr) {
    }

    SynchVector(std::size_t _size) : capacity(_size), size(_size), data(new T[_size]) {
    }

    SynchVector(const SynchVector& vec) : capacity(vec.capacity), size(vec.size)
                              , data(static_cast<T*>(::operator new(sizeof(T) * capacity))) {
        for (std::size_t i = 0; i < size; ++i) {
            new(data + i) T(vec.data[i]);
        }
    }

    ~SynchVector() {
        clear();
    }

    T& operator[](std::size_t index) {
        std::lock_guard<std::mutex> guard(resource);
        assert(index < size);
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        std::lock_guard<std::mutex> guard(resource);
        assert(index < size);
        return data[index];
    }

    bool operator==(const SynchVector& other) {
        if(size != other.size)
            return false;
        for(std::size_t i = 0; i < size; i++) {
            if(data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }

    void clear() {
        std::lock_guard<std::mutex> guard(resource);
        for (std::size_t i = 0; i < size; i++)
            data[i].~T();
        operator delete (data);
        data = nullptr;
        capacity = 0;
        size = 0;
    }

    void push_back(const T& v) {
        std::lock_guard<std::mutex> guard(resource);
        if (capacity == size) {
            capacity = (capacity * 2 + 1);

            T* tmp = static_cast<T*>(::operator new(sizeof(T) * capacity));
            for (std::size_t i = 0; i < size; ++i) {
                new (tmp + i) T(std::move(data[i]));
                data[i].~T();
            }
            operator delete (data);
            data = tmp;
        }
        data[size] = v;
        ++size;
    }

    void pop_back() {
        std::lock_guard<std::mutex> guard(resource);
        if (size > 0) {
            size--;
            data[size].~T();
        }
    }

    void reserve(std::size_t _capacity) {
        std::lock_guard<std::mutex> guard(resource);
        if (_capacity > capacity) {
            capacity = _capacity;
            T* new_data = static_cast<T*>(::operator new(sizeof(T) * capacity));
            for (std::size_t i = 0; i < size; i++) {
                new (new_data + i) T(std::move(data[i]));
                data[i].~T();
            }
            operator delete (data);
            data = new_data;
        }
    }

private:
    std::mutex resource;
    std::size_t capacity;
    std::size_t size;
    T* data;
};

template<class T>
class AtomicsVector {
public:
    AtomicsVector() : capacity(0), size(0), data(nullptr) {
    }

    AtomicsVector(std::size_t _size, T value = T{}) : capacity(_size), size(_size) {
        data = new std::atomic<T>[size];
        for(std::size_t i = 0; i < size; i++) {
            std::atomic_init(&data[i], value);
        }
    }

    AtomicsVector(const AtomicsVector& other) {
        std::atomic_init(&size, other.size.load());
        std::atomic_init(&capacity, other.capacity.load());
        data = static_cast<std::atomic<T>*>(::operator new(sizeof(std::atomic<T>) * capacity.load()));
        for (std::size_t i = 0; i < size; ++i) {
            std::atomic_init(&data[i], other.data[i].load());
        }
    }

    ~AtomicsVector() {
        clear();
    }

    std::atomic<T>& operator[](std::size_t index) {
        return data[index];
    }

    const std::atomic<T>& operator[](std::size_t index) const {
        return data[index];
    }

    bool operator==(const AtomicsVector& other) {
        if(size != other.size)
            return false;
        for(std::size_t i = 0; i < size; i++) {
            if(data[i] != other.data[i])
                return false;
        }
        return true;
    }

    void resize(std::size_t new_size) {
        std::atomic_init(&size, new_size);
        std::atomic_init(&capacity, new_size);
        data = static_cast<std::atomic<T>*>(::operator new(sizeof(std::atomic<T>) * capacity.load()));
        for (std::size_t i = 0; i < size; ++i) {
            std::atomic_init(&data[i], T());
        }
    }

    void clear() {
        delete[] data;
        data = nullptr;
        capacity = 0;
        size = 0;
    }

    std::size_t get_size() const {
        return size;
    }

private:
    std::atomic<std::size_t> capacity;
    std::atomic<std::size_t> size;
    std::atomic<T>* data;
};