#include "matrix_op.h"
#include "matrix_reference.h"
#include <random>

void MatrixOp::getAddReference(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::size_t count) {
    auto N = A.size();
    for(std::size_t c = 0; c < count; c++) {
        MatrixOp::EltwiseAddParams(A, B, 0, N);
    }
}

void MatrixOp::getSubReference(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::size_t count) {
    auto N = A.size();
    for(std::size_t c = 0; c < count; c++) {
        MatrixOp::EltwiseSubParams(A, B, 0, N);
    }
}

void MatrixOp::MatMul(std::vector<AtomicsVector<int>>& A, 
            const std::vector<std::vector<int>>& B, const std::vector<std::vector<int>>& C){
    auto N = A.size();
    auto M = A[0].get_size();
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            int cur = 0;
            for(std::size_t k = 0; k < M; k++) {
                cur += B[i][k] * C[k][j];
            }
            A[i][j] = cur;
        }
    }
}

void MatrixOp::MatMulLinear(std::vector<int>& A, 
                  const std::vector<int>& B, const std::vector<int>& C){
    auto size = static_cast<std::size_t>(sqrt(A.size()));
    for(std::size_t i = 0; i < size; i++) {
        for(std::size_t j = 0; j < size; j++) {
            int cur = 0;
            for(std::size_t k = 0; k < size; k++) {
                cur += B[i * size + k] * C[k * size + j];
            }
            A[i * size + j] += cur;
        }
    }
}
