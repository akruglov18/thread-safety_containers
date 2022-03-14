#include "vector.h"
#include <vector>
#include "common.h"
#include <thread>
#include <gtest/gtest.h>
#include <memory>
#include <iostream>

TEST(Vector, simultaneous_0) {
    std::size_t size = 1000;
    Vector<int> copy_a(size);
    Vector<int> a(size);
    int count = 100000;
    auto incr = [&](std::size_t ind) {
        for(int j = 0; j < count; j++) {
            a[ind]++;
        }
    };
    auto decr = [&](std::size_t ind) {
        for(int j = 0; j < count; j++) {
            a[ind]--;
        }
    };
    auto start = std::chrono::high_resolution_clock::now();
    for(std::size_t i = 0; i < a.get_size(); i++) {
        std::thread t1(incr, i);
        std::thread t2(decr, i);
        t1.join();
        t2.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "execution time: " << time << " ms\n";
    ASSERT_TRUE(a == copy_a);
}

// void increment(Vector<int>& a, int count, std::size_t ind) {
//     for(int j = 0; j < count; j++) {
//         a[ind]++;
//     }
//     std::cout << a[ind] << " ";
// }

// void decrement(Vector<int>& a, int count, std::size_t ind) {
//     for(int j = 0; j < count; j++) {
//         // a[ind]--;
//     }
// }

// TEST(Vector, simultaneous_1) {
//     std::size_t size = 1000;
//     Vector<int> copy_a(size);
//     Vector<int> a(size);
//     int count = 100;
//     auto start = std::chrono::high_resolution_clock::now();
//     for(std::size_t i = 0; i < a.get_size(); i++) {
//         std::thread t1(increment, a, count, i);
//         std::thread t2(decrement, a, count, i);
//         t1.join();
//         t2.join();
//         std::cout << a[i] << " ";
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     std::cout << "execution time: " << time << " ms\n";
//     ASSERT_TRUE(a == copy_a);
// }

TEST(Common, simultaneous_2) {
    std::size_t size = 1000;
    std::vector<int> copy_a(size);
    std::vector<int> a(size);
    Common vector_a(a);
    int count = 100000;
    auto incr = [&](std::size_t ind) {
        for(int j = 0; j < count; j++) {
            vector_a.lock();
            vector_a.get()[ind]++;
            vector_a.unlock();
        }
    };
    auto decr = [&](std::size_t ind) {
        for(int j = 0; j < count; j++) {
            vector_a.lock();
            vector_a.get()[ind]--;
            vector_a.unlock();
        }
    };
    auto start = std::chrono::high_resolution_clock::now();
    for(std::size_t i = 0; i < a.size(); i++) {
        std::thread t1(incr, i);
        std::thread t2(decr, i);
        t1.join();
        t2.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "execution time: " << time << " ms\n";
    ASSERT_TRUE(a == copy_a);
}
