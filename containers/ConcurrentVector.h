#pragma once

#include <mutex>
#include <list>
#include <atomic>

class Accessor {
public:
    std::atomic<uint32_t>& counter;
    Accessor(std::atomic<uint32_t>& value) : counter(value) {
        counter++;
    }

    ~Accessor() {
        counter--;
    }
};

template<typename T, size_t block_size>
class MemoryKeeper {
public:
    std::atomic<uint32_t> requestsCount;
    size_t size;
    T** data;

public:
    MemoryKeeper() : requestsCount(0), size(0), data(nullptr)
    {
    }

    MemoryKeeper(const MemoryKeeper& other) : requestsCount(0), size(other.size), data(nullptr) {
        if (size > 0) {
            data = new T * [size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
    }

    ~MemoryKeeper() {
        data = nullptr;
    }

    void addNewBlock() {
        T** newData = new T*[size + 1];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        T* newBLock = new T[block_size];
        data[size] = newBLock;
        size++;
    }

    void clearMemory() {
        for (size_t i = 0; i < size; i++) {
            delete[] data[i];
        }
        delete[] data;
        size = 0;
    }

    T& operator[](size_t i) {
        Accessor accessor(requestsCount);
        return data[i / block_size][i % block_size];
    }

    const T& operator[](size_t i) const {
        Accessor accessor(requestsCount);
        return data[i / block_size][i % block_size];
    }
};

template<typename T, size_t block_size>
class concurrent_vector {
private:
    MemoryKeeper<T, block_size>* memKeeper;
    size_t size;
    size_t capacity;
    std::mutex growth;
    std::atomic<uint32_t> requestsCount;

public:
    concurrent_vector() : memKeeper(new MemoryKeeper<T, block_size>()), size(0), capacity(0) {
    }

    void push_back(const T& val) {
        growth.lock();
        Accessor accessor(requestsCount);
        if (size == capacity) {
            MemoryKeeper<T, block_size>* newMemKeeper = new MemoryKeeper<T, block_size>(*memKeeper);
            newMemKeeper->addNewBlock();
            MemoryKeeper<T, block_size>* memKeeperCopy = memKeeper;
            memKeeper = newMemKeeper;
            capacity += block_size;
            std::atomic<uint32_t> newCounter;

            while (memKeeperCopy->requestsCount != 0) {}

            delete memKeeperCopy;
        }
        size_t num_elem = size;
        size++;
        growth.unlock();
        (*memKeeper)[num_elem] = val;
    }

    T& operator[](std::size_t i) {
        Accessor accessor(requestsCount);
        return (*memKeeper)[i];
    }

    const T& operator[](std::size_t i) const {
        Accessor accessor(requestsCount);
        return (*memKeeper)[i];
    }

    void clear() {
        size = 0;
    }

    size_t get_size() const {
        return size;
    }

    ~concurrent_vector() {
        memKeeper->clearMemory();
        delete memKeeper;
    }
};
