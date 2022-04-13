#pragma once
#include "vector.h"
#include <vector>

namespace MatrixOp {

void getAddReference(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::size_t count);

void getSubReference(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::size_t count);

void MatMul(std::vector<AtomicsVector<int>>& A, 
            const std::vector<std::vector<int>>& B, const std::vector<std::vector<int>>& C);

void MatMulLinear(std::vector<int>& A, 
                  const std::vector<int>& B, const std::vector<int>& C);

} // MatrixOp
