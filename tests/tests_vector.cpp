#include "vector.h"
#include <vector>
#include "common.h"
#include <thread>
#include <gtest/gtest.h>
#include <iostream>

namespace {

std::vector<std::size_t> sizes = {500, 1000, 1500, 2000};
std::vector<std::size_t> counts = {1000, 10000, 100000};

typedef testing::TestWithParam<std::tuple<std::size_t, std::size_t>> AtomicVector;
TEST_P(AtomicVector, IncrAndDecr) {
    auto params = GetParam();
    auto size = std::get<0>(params);
    auto count = std::get<1>(params);
    AtomicsVector<int> a(size, 0);
    auto incr = [&](std::size_t ind) {
        for(std::size_t j = 0; j < count; j++) {
            a[ind]++;
        }
    };
    auto decr = [&](std::size_t ind) {
        for(std::size_t j = 0; j < count; j++) {
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
    std::cout << "size: " << size << "\n";
    std::cout << "count: " << count << "\n";
    std::cout << "execution time: " << time << " ms\n";
    for(std::size_t i = 0; i < a.get_size(); i++) {
        EXPECT_EQ(a[i], 0);
    }
}

INSTANTIATE_TEST_SUITE_P(/**/, AtomicVector, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(counts)
    )
);

typedef testing::TestWithParam<std::tuple<std::size_t, std::size_t>> CommonVector;
TEST_P(CommonVector, IncrAndDecr1) {
    auto params = GetParam();
    auto size = std::get<0>(params);
    auto count = std::get<1>(params);
    std::vector<int> vect(size);
    Common a(vect);
    auto incr = [&](std::size_t ind) {
        auto& obj = a.get();
        for(std::size_t j = 0; j < count; j++) {
            std::lock_guard<std::mutex> guard(a.get_mutex());
            obj[ind]++;
        }
    };
    auto decr = [&](std::size_t ind) {
        auto& obj = a.get();
        for(std::size_t j = 0; j < count; j++) {
            std::lock_guard<std::mutex> guard(a.get_mutex());
            obj[ind]--;
        }
    };
    auto start = std::chrono::high_resolution_clock::now();
    for(std::size_t i = 0; i < size; i++) {
        std::thread t1(incr, i);
        std::thread t2(decr, i);
        t1.join();
        t2.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "size: " << size << "\n";
    std::cout << "count: " << count << "\n";
    std::cout << "execution time: " << time << " ms\n";
    for(std::size_t i = 0; i < size; i++) {
        EXPECT_EQ(vect[i], 0);
    }
}

INSTANTIATE_TEST_SUITE_P(/**/, CommonVector, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(counts)
    )
);

} // namespace
