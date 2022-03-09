#include "vector.h"
#include <thread>
#include <gtest/gtest.h>
#include <memory>

std::size_t size = 1000;
Vector<int> a(size);

void increment(std::size_t ind, int count){
    for(int i = 0; i < count; i++) {
        a[ind]++;
    }
}

void decrement(std::size_t ind, int count) {
    for(int i = 0; i < count; i++) {
        a[ind]--;
    }
}

TEST(Vector, assymetric) {
    Vector<int> copy_a(size);
    int count = 100000;
    for(std::size_t i = 0; i < a.get_size(); i++) {
        std::thread t1(increment, i, count);
        std::thread t2(decrement, i, count);
        t1.join();
        t2.join();
    }
    ASSERT_TRUE(a == copy_a);
}
