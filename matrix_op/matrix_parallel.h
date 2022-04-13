#pragma once

#include "common.h"
#include "vector.h"

namespace MatrixOp {

void getAddParallel(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                    std::size_t count, std::size_t thread_count);

void getAddParallelAtomic(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count);

void getAddParallelCommon(Common<std::vector<std::vector<int>>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count);

void getSubParallel(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                    std::size_t count, std::size_t thread_count);

void getSubParallelAtomic(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count);

} // MatrixOp
