#include "ConcurrentVector.h"
#include <iostream>


int main() {
    std::cout << 0 << std::endl;
    concurrent_vector<int, 2> v;
    v.push_back(1);
    std::cout << v[0] << std::endl;
}
