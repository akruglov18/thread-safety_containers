// #include "vector.h"
// #include <thread>
// #include <gtest/gtest.h>
// #include <memory>

// void increment(Vector<int>& a, std::size_t ind, int count){
//     for(int i = 0; i < count; i++) {
//         a[ind]++;
//     }
// }

// void decrement(Vector<int>& a, std::size_t ind, int count) {
//     for(int i = 0; i < count - 1; i++) {
//         a[ind]--;
//     }
// }

// TEST(Vector, assymetric) {
//     std::size_t size = 1000;
//     Vector<int> a(size);
//     Vector<int> copy_a(size);
//     int count = 1000000;
//     for(std::size_t i = 0; i < a.get_size(); i++) {
//         std::thread t1(increment, a, i, count);
//         std::thread t2(decrement, a, i, count);
//         t1.join();
//         t2.join();
//     }
//     ASSERT_TRUE(a == copy_a);
// }
