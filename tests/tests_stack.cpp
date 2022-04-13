#include "stack.h"
#include <gtest/gtest.h>
#include <bitset>
#include <thread>

TEST(Stack, statistics) {
constexpr int size = 1000000;
    Stack<int> s(size);
    auto pusher = [&](int start, int finish) {
        for(int i = start; i < finish; i++) {
            s.push(i);
        }
    };
    std::thread t1(pusher, 0, size / 2);
    std::thread t2(pusher, size / 2, size);
    t1.join();
    t2.join();
    std::bitset<size> bs;
    for(int i = 0; i < size; i++) {
        int x = s.top();
        EXPECT_TRUE(x >= 0 && x < size);
        bs.set(x, true);
    }
    EXPECT_TRUE(bs.all());
}