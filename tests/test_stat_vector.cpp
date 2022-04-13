#include <gtest/gtest.h>
#include "vector.h"
#include <iostream>
#include <thread>
#include <memory>
#include <bitset>

TEST(SynchVector, statistics) {
    constexpr int size = 1000000;
    SynchVector<int> vec;
    vec.reserve(size);
    auto pusher = [&](int start, int finish) {
        for(int i = start; i < finish; i++) {
            vec.push_back(i);
        }
    };
    std::thread t1(pusher, 0, size / 2);
    std::thread t2(pusher, size / 2, size);
    t1.join();
    t2.join();
    std::bitset<size> bs;
    for(int i = 0; i < size; i++) {
        EXPECT_TRUE(vec[i] >= 0 && vec[i] < size);
        bs.set(vec[i], true);
    }
    EXPECT_TRUE(bs.all());
}