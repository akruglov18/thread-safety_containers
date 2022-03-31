#pragma once
#include "vector.h"
#include <vector>
#include <random>

namespace Eltwise {

void EltwiseAdd(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto size = A.size();
    for(std::size_t i = 0; i < size; i++) {
        for(std::size_t j = 0; j < size; j++) {
            A[i][j] += B[i][j];
        }
    }
}

void EltwiseSub(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B) {
auto size = A.size();
    for(std::size_t i = 0; i < size; i++) {
        for(std::size_t j = 0; j < size; j++) {
            A[i][j] -= B[i][j];
        }
    }
}

void GenerateMatrix(std::vector<std::vector<int>>& matr) {
    auto N = matr.size();
    auto M = matr[0].size();
    std::random_device device;
    std::mt19937 gen(device());
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            matr[i][j] = gen() % 100;
        }
    }
}

} // Eltwise
