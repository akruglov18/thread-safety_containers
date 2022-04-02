#pragma once
#include "matrix_op.h"
#include "vector.h"
#include <vector>
#include <thread>
#include <memory>

namespace MatrixOp {

void getAddParallel(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                    std::size_t count, std::size_t thread_count) {
    auto rows_per_thread = B.size() / thread_count;
    for(std::size_t c = 0; c < count; c++) {
        std::size_t start = 0, finish = rows_per_thread;
        std::vector<std::shared_ptr<std::thread>> thread_group(thread_count);
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i] = std::make_shared<std::thread>(
                              std::thread(EltwiseAddParams, std::ref(A), std::ref(B), start, finish));
            start = finish;
            finish += rows_per_thread;
        }
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i]->join();
        }
    }
}

void getAddParallelAtomic(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(count);
    for(size_t i = 0; i < count; i++) {
        thread_group[i] = std::make_shared<std::thread>
                          (std::thread (EltwiseAdd, std::ref(A), std::ref(B)));
    }
    for(size_t i = 0; i < count; i++) {
        thread_group[i]->join();
    }
}

void getAddParallelCommon(Common<std::vector<std::vector<int>>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(count);
    for(size_t i = 0; i < count; i++) {
        thread_group[i] = std::make_shared<std::thread>
                          (std::thread (EltwiseAddCommon, std::ref(A), std::ref(B)));
    }
    for(size_t i = 0; i < count; i++) {
        thread_group[i]->join();
    }
}

void getSubParallel(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                    std::size_t count, std::size_t thread_count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(thread_count);
    auto rows_per_thread = B.size() / thread_count;
    for(std::size_t c = 0; c < count; c++) {
        std::size_t start = 0, finish = rows_per_thread;
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i] = std::make_shared<std::thread>(
                              std::thread (EltwiseSubParams, std::ref(A), std::ref(B), start, finish));
            start = finish;
            finish += rows_per_thread;
        }
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i]->join();
        }
    }
}

void getSubParallelAtomic(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(count);
    for(size_t i = 0; i < count; i++) {
        thread_group[i] = std::make_shared<std::thread>
                          (std::thread (EltwiseSub, std::ref(A), std::ref(B)));
    }
    for(size_t i = 0; i < count; i++) {
        thread_group[i]->join();
    }
}

} // MatrixOp
