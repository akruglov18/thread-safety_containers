#include "vector.h"
#include <vector>
#include <random>
#include "common.h"

namespace MatrixOp {

void EltwiseAdd(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto N = A.size();
    auto M = A[0].get_size();
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            A[i][j] += B[i][j];
        }
    }
}

void EltwiseAddCommon(Common<std::vector<std::vector<int>>>& A, const std::vector<std::vector<int>>& B) {
    auto N = A.get().size();
    auto M = A.get()[0].size();
    auto& obj = A.get();
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            std::lock_guard<std::mutex> guard(A.get_mutex());
            obj[i][j] += B[i][j];
        }
    }
}

void EltwiseAddParams(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                      std::size_t start_row, std::size_t finish_row) {
    auto size = A[0].size();
    for(std::size_t i = start_row; i < finish_row; i++) {
        for(std::size_t j = 0; j < size; j++) {
            A[i][j] += B[i][j];
        }
    }
}

void EltwiseSub(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto N = A.size();
    auto M = A[0].get_size();
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            A[i][j] -= B[i][j];
        }
    }
}

void EltwiseSubParams(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                      std::size_t start_row, std::size_t finish_row) {
    auto M = A[0].size();
    for(std::size_t i = start_row; i < finish_row; i++) {
        for(std::size_t j = 0; j < M; j++) {
            A[i][j] -= B[i][j];
        }
    }
}

void GenerateMatrix(std::vector<std::vector<int>>& matr, int mod = 100) {
    auto N = matr.size();
    auto M = matr[0].size();
    std::random_device device;
    std::mt19937 gen(device());
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            matr[i][j] = gen() % mod;
        }
    }
}

} // MatrixOp
