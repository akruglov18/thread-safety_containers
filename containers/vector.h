#include "utils.h"
#include <iostream>
#include <mutex>
#include <cassert>
#include <atomic>

template<class T, class Enable = void>
class Vector {
public:
    Vector() : capacity(0), size(0), data(nullptr) {
        std::cout << "Primary\n";
    }

    Vector(const Vector& vec) : capacity(vec.capacity), size(vec.size)
                              , data(static_cast<T*>(::operator new(sizeof(T) * capacity))) {
        for (std::size_t i = 0; i < size; ++i) {
            new(data + i) T(vec.data[i]);
        }
    }

    ~Vector() {
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

    std::size_t get_size() const {
        std::lock_guard<std::mutex> guard(resource);
        return size;
    }

private:
    std::mutex resource;
    std::size_t capacity;
    std::size_t size;
    T* data;
};

template<class T>
class Vector<T, typename std::enable_if<is_atomic<T>::value>::type> {
public:
        Vector() : capacity(0), size(0), data(nullptr) {
        std::cout << "Atomic\n";
    }

    Vector(const Vector& other) : capacity(other.capacity), size(other.size)
                              , data(new T[other.size]) {
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    ~Vector() {
        clear();
    }

    T& operator[](std::size_t index) {
        assert(index < size);
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        assert(index < size);
        return data[index];
    }

    void clear() {
        delete[] data;
        data = nullptr;
        capacity = 0;
        size = 0;
    }

    void push_back(const T& v) {
        if (capacity == size) {
            capacity = (capacity * 2 + 1);

            T* tmp = new T[capacity];
            for (std::size_t i = 0; i < size; ++i) {
                tmp[i] = std::move(data[i]);
            }
            delete[] data;
            data = tmp;
        }
        data[size] = v;
        ++size;
    }

    void pop_back() {
        if (size > 0) {
            size--;
        }
    }

    void reserve(std::size_t _capacity) {
        if (_capacity > capacity) {
            capacity = _capacity;
            T* new_data = new T[capacity];
            for (std::size_t i = 0; i < size; i++) {
                new_data[i] = std::move(data[i]);
            }
            delete[] data;
            data = new_data;
        }
    }

    std::size_t get_size() const {
        return size;
    }

private:
    std::atomic<std::size_t> capacity;
    std::atomic<std::size_t> size;
    std::atomic<T>* data;
};