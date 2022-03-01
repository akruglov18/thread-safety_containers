#include "vector.h"
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

Vector<int> a;

void incr(int ind, int count) {
    for (int i  = 0; i < count; i++)
        a[ind]++;
}

void decr(int ind, int count) {
    for (int i = 0; i < count; i++)
        a[ind]--;
}

int main() {
    std::size_t size = 10;
    a.resize(10);
    int count = 1000000;
    for(int i = 0; i < size; i++) {
        std::thread t1(incr, i, count);
        std::thread t2(decr, i, count);
        t1.join();
        t2.join();
    }
    for (std::size_t i = 0; i < size; i++) {
        std::cout << a[i] << " ";
    }
}
