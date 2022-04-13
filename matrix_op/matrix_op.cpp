#include <random>
#include "matrix_op.h"

void MatrixOp::EltwiseAdd(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto N = A.size();
    auto M = A[0].get_size();
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            A[i][j] += B[i][j];
        }
    }
}

void MatrixOp::EltwiseAddReverse(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto N = static_cast<int>(A.size());
    auto M = A[0].get_size();
    for(int i = N - 1; i >= 0; i--) {
        for(std::size_t j = 0; j < M; j++) {
            A[i][j] += B[i][j];
        }
    }
}

void MatrixOp::EltwiseAddCommon(Common<std::vector<std::vector<int>>>& A, const std::vector<std::vector<int>>& B) {
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

void MatrixOp::EltwiseAddParams(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                      std::size_t start_row, std::size_t finish_row) {
    auto size = A[0].size();
    for(std::size_t i = start_row; i < finish_row; i++) {
        for(std::size_t j = 0; j < size; j++) {
            A[i][j] += B[i][j];
        }
    }
}

void MatrixOp::EltwiseSub(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto N = A.size();
    auto M = A[0].get_size();
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            A[i][j] -= B[i][j];
        }
    }
}

void MatrixOp::EltwiseSubParams(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                      std::size_t start_row, std::size_t finish_row) {
    auto M = A[0].size();
    for(std::size_t i = start_row; i < finish_row; i++) {
        for(std::size_t j = 0; j < M; j++) {
            A[i][j] -= B[i][j];
        }
    }
}

void MatrixOp::GenerateMatrix(std::vector<std::vector<int>>& matr, int mod) {
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
