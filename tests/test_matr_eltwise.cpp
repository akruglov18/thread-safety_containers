#include <vector>
#include "matrix_op.h"
#include <gtest/gtest.h>
#include "vector.h"
#include <thread>
#include <memory>

namespace {

using namespace std;

vector<size_t> sizes = {1000, 3000, 5000, 7000};
vector<size_t> matr_count = {6, 12};

typedef vector<AtomicsVector<int>> MatrixInt;

typedef testing::TestWithParam<tuple<size_t, size_t>> AtomicsVectorParams;
TEST_P(AtomicsVectorParams, EltwiseSumParallel) {
    auto params = GetParam();
    auto size = get<0>(params);
    auto matrix_count = get<1>(params);

    MatrixInt res(size, AtomicsVector<int>(size));
    vector<vector<int>> added_matrix(size, vector<int>(size));
    Eltwise::GenerateMatrix(added_matrix);
    vector<shared_ptr<thread>> thread_group(matrix_count);

    auto start = chrono::high_resolution_clock::now();
    for(size_t i = 0; i < matrix_count; i++) {
        thread_group[i] = make_shared<thread>(thread (Eltwise::EltwiseAdd, std::ref(res), std::ref(added_matrix)));
    }
    for(size_t i = 0; i < matrix_count; i++) {
        thread_group[i]->join();
    }
    auto end = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "size: " << size << "\n";
    cout << "matrix_count: " << matrix_count << "\n";
    cout << "simultaneous execution time: " << time << " ms\n";

    int multiply = static_cast<int>(matrix_count);
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            ASSERT_EQ(res[i][j], multiply * added_matrix[i][j]);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(/**/, AtomicsVectorParams, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(matr_count)
    )
);

typedef testing::TestWithParam<tuple<size_t, size_t>> CommonVectorParams;
TEST_P(CommonVectorParams, EltwiseSumLinear) {
    auto params = GetParam();
    auto size = get<0>(params);
    auto matrix_count = get<1>(params);

    vector<vector<int>> added_matrix(size, vector<int>(size));
    Eltwise::GenerateMatrix(added_matrix);
    vector<vector<int>> res(size, vector<int>(size));

    auto start = chrono::high_resolution_clock::now();

    for(std::size_t c = 0; c < matrix_count; c++) {
        for(std::size_t i = 0; i < size; i++) {
            for(std::size_t j = 0; j < size; j++) {
                res[i][j] += added_matrix[i][j];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "size: " << size << "\n";
    cout << "matrix_count: " << matrix_count << "\n";
    cout << "simultaneous execution time: " << time << " ms\n";

    int multiply = static_cast<int>(matrix_count);
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            ASSERT_EQ(res[i][j], multiply * added_matrix[i][j]);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(/**/, CommonVectorParams, 
    testing::Combine(
        testing::ValuesIn(sizes),
        testing::ValuesIn(matr_count)
    )
);

}
