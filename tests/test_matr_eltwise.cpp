#include <vector>
#include <gtest/gtest.h>
#include "vector.h"
#include <iostream>
#include <thread>
#include <memory>
#include "matrix_op.h"
#include "matrix_parallel.h"
#include "matrix_reference.h"
#include "common.h"

namespace {

std::vector<size_t> sizes = {1008, 3000, 5004, 7008};
std::vector<size_t> matr_count = {6, 12};

typedef testing::TestWithParam<std::tuple<size_t, size_t>> ParallelParams;
TEST_P(ParallelParams, EltwiseSumParallel) {
    auto params = GetParam();
    auto size = std::get<0>(params);
    auto matrix_count = std::get<1>(params);

    std::vector<std::vector<int>> res(size, std::vector<int>(size, 0));
    std::vector<std::vector<int>> added_matrix(size, std::vector<int>(size));
    MatrixOp::GenerateMatrix(added_matrix);

    auto thread_count = matrix_count;
    auto start = std::chrono::high_resolution_clock::now();
    MatrixOp::getAddParallel(std::ref(res), added_matrix, matrix_count, thread_count);
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "size: " << size << "\n";
    std::cout << "matrix count: " << matrix_count << "\n";
    std::cout << "execution time: " << time << " ms\n";

    auto multiplier = static_cast<int>(matrix_count);
    for(std::size_t i = 0; i < size; i++) {
        for(std::size_t j = 0; j < size; j++) {
            ASSERT_EQ(res[i][j], added_matrix[i][j] * multiplier);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(/**/, ParallelParams, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(matr_count)
    )
);

typedef testing::TestWithParam<std::tuple<size_t, size_t>> ParallelAtomicParams;
TEST_P(ParallelAtomicParams, EltwiseSumParallelAtomic) {
    auto params = GetParam();
    auto size = std::get<0>(params);
    auto matrix_count = std::get<1>(params);

    std::vector<AtomicsVector<int>> res(size, AtomicsVector<int>(size));
    std::vector<std::vector<int>> added_matrix(size, std::vector<int>(size));
    MatrixOp::GenerateMatrix(added_matrix);

    auto start = std::chrono::high_resolution_clock::now();
    MatrixOp::getAddParallelAtomic(res, added_matrix, matrix_count);
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "size: " << size << "\n";
    std::cout << "matrix_count: " << matrix_count << "\n";
    std::cout << "execution time: " << time << " ms\n";

    auto multiplier = static_cast<int>(matrix_count);
    for(std::size_t i = 0; i < size; i++) {
        for(std::size_t j = 0; j < size; j++) {
            ASSERT_EQ(res[i][j], added_matrix[i][j] * multiplier);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(/**/, ParallelAtomicParams, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(matr_count)
    )
);

typedef testing::TestWithParam<std::tuple<size_t, size_t>> ParallelCommonParams;
TEST_P(ParallelCommonParams, EltwiseSumParallelCommon) {
    auto params = GetParam();
    auto size = std::get<0>(params);
    auto matrix_count = std::get<1>(params);

    std::vector<std::vector<int>> res(size, std::vector<int>(size));
    std::vector<std::vector<int>> added_matrix(size, std::vector<int>(size));
    MatrixOp::GenerateMatrix(added_matrix);
    Common<std::vector<std::vector<int>>> result_obj(res);

    auto start = std::chrono::high_resolution_clock::now();
    MatrixOp::getAddParallelCommon(result_obj, added_matrix, matrix_count);
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "size: " << size << "\n";
    std::cout << "matrix_count: " << matrix_count << "\n";
    std::cout << "execution time: " << time << " ms\n";

    auto multiplier = static_cast<int>(matrix_count);
    for(std::size_t i = 0; i < size; i++) {
        for(std::size_t j = 0; j < size; j++) {
            ASSERT_EQ(res[i][j], added_matrix[i][j] * multiplier);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(/**/, ParallelCommonParams, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(matr_count)
    )
);

typedef testing::TestWithParam<std::tuple<size_t, size_t>> LinearParams;
TEST_P(LinearParams, EltwiseSumLinear) {
    auto params = GetParam();
    auto size = std::get<0>(params);
    auto matrix_count = std::get<1>(params);

    std::vector<std::vector<int>> res(size, std::vector<int>(size));
    std::vector<std::vector<int>> added_matrix(size, std::vector<int>(size));
    MatrixOp::GenerateMatrix(added_matrix);

    auto start = std::chrono::high_resolution_clock::now();

    MatrixOp::getAddReference(res, added_matrix, matrix_count);

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "size: " << size << "\n";
    std::cout << "matrix_count: " << matrix_count << "\n";
    std::cout << "execution time: " << time << " ms\n";

}

INSTANTIATE_TEST_SUITE_P(/**/, LinearParams, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(matr_count)
    )
);

}
