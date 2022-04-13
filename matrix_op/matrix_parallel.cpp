#include <vector>
#include <thread>
#include <memory>
#include "matrix_parallel.h"
#include "matrix_reference.h"
#include "matrix_op.h"

void MatrixOp::getAddParallel(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                    std::size_t count, std::size_t thread_count) {
    auto rows_per_thread = B.size() / thread_count;
    for(std::size_t c = 0; c < count; c++) {
        std::size_t start = 0, finish = rows_per_thread;
        std::vector<std::shared_ptr<std::thread>> thread_group(thread_count);
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i] = std::make_shared<std::thread>(
                              std::thread(MatrixOp::EltwiseAddParams, std::ref(A), std::ref(B), start, finish));
            start = finish;
            finish += rows_per_thread;
        }
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i]->join();
        }
    }
}

void MatrixOp::getAddParallelAtomic(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(count);
    for(size_t i = 0; i < count; i++) {
        if(i & 1) {
            thread_group[i] = std::make_shared<std::thread>
                              (std::thread (MatrixOp::EltwiseAddReverse, std::ref(A), std::ref(B)));
        } else {
            thread_group[i] = std::make_shared<std::thread>
                              (std::thread (MatrixOp::EltwiseAdd, std::ref(A), std::ref(B)));
        }
    }
    for(size_t i = 0; i < count; i++) {
        thread_group[i]->join();
    }
}

void MatrixOp::getAddParallelCommon(Common<std::vector<std::vector<int>>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(count);
    for(size_t i = 0; i < count; i++) {
        thread_group[i] = std::make_shared<std::thread>
                          (std::thread (MatrixOp::EltwiseAddCommon, std::ref(A), std::ref(B)));
    }
    for(size_t i = 0; i < count; i++) {
        thread_group[i]->join();
    }
}

void MatrixOp::getSubParallel(std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                    std::size_t count, std::size_t thread_count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(thread_count);
    auto rows_per_thread = B.size() / thread_count;
    for(std::size_t c = 0; c < count; c++) {
        std::size_t start = 0, finish = rows_per_thread;
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i] = std::make_shared<std::thread>(
                              std::thread(MatrixOp::EltwiseSubParams, std::ref(A), std::ref(B), start, finish));
            start = finish;
            finish += rows_per_thread;
        }
        for(size_t i = 0; i < thread_count; i++) {
            thread_group[i]->join();
        }
    }
}

void MatrixOp::getSubParallelAtomic(std::vector<AtomicsVector<int>>& A, const std::vector<std::vector<int>>& B, 
                          std::size_t count) {
    std::vector<std::shared_ptr<std::thread>> thread_group(count);
    for(size_t i = 0; i < count; i++) {
        thread_group[i] = std::make_shared<std::thread>
                          (std::thread (MatrixOp::EltwiseSub, std::ref(A), std::ref(B)));
    }
    for(size_t i = 0; i < count; i++) {
        thread_group[i]->join();
    }
}
