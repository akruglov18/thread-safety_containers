#pragma once
#include "matrix_op.h"
#include "vector.h"
#include <vector>
#include <random>

namespace MatrixOp {

void getAddReference(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::size_t count) {
    auto N = A.size();
    for(std::size_t c = 0; c < count; c++) {
        EltwiseAddParams(A, B, 0, N);
    }
}

void getSubReference(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::size_t count) {
    auto N = A.size();
    for(std::size_t c = 0; c < count; c++) {
        EltwiseSubParams(A, B, 0, N);
    }
}

void MatMul(std::vector<AtomicsVector<int>>& A, 
            const std::vector<std::vector<int>>& B, const std::vector<std::vector<int>>& C){
    auto N = A.size();
    auto M = A[0].get_size();
    for(std::size_t i = 0; i < N; i++) {
        for(std::size_t j = 0; j < M; j++) {
            int cur = 0;
            for(std::size_t k = 0; k < M; k++) {
                cur += B[i][k] + C[k][j];
            }
            A[i][j] = cur;
        }
    }
    
}

} // MatrixOp
