#pragma once
#include "vector.h"
#include <vector>
#include "common.h"

namespace MatrixOp {

void EltwiseAdd(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B);
void EltwiseAddReverse(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B);
void EltwiseAddParams(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                      std::size_t start_row, std::size_t finish_row);
void EltwiseAddCommon(Common<std::vector<std::vector<int>>>& A, const std::vector<std::vector<int>>& B);

void EltwiseSub(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B);
void EltwiseSubParams(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                      std::size_t start_row, std::size_t finish_row);
void GenerateMatrix(std::vector<std::vector<int>>& matr, int mod = 100);

} // MatrixOp
