#pragma once
#include <mutex>

struct Accessor {
    std::atomic<int>& counter;
    Accessor(std::atomic<int>& counter) : counter(counter) {

    }

    ~Accessor() {
        counter--;
    }
};

class Vector {
private:
    int* data;
    int size;
    int capacity;
    bool flag;
    std::atomic<int> cnt;
    std::mutex growth;
public:
    Vector() : data(nullptr), size(0), capacity(0), flag(false), cnt(0) {

    }

    void push_back(const int& value) {
        growth.lock();
        flag = true;
        while (cnt != 0) {}

        if (size == capacity) {
            int new_capacity = capacity * 2 + 1;
            int* new_data = new int[new_capacity];
            for (int i = 0; i < size; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }

        data[size] = value;
        ++size;

        flag = false;
        growth.unlock();
    }

    size_t get_size() const {
        return size;
    }

    int& operator[](size_t i) {
        Accessor accessor(cnt);
        while (true) {
            while (flag) {}
            cnt++;
            if (flag)
                cnt--;
            else
                break;
        }
        return data[i];
    }

    ~Vector() {
        delete[] data;
    }
};