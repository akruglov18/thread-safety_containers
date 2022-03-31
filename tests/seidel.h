#pragma once
#include "vector.h"

typedef std::vector<std::vector<double>> matrix;

AtomicsVector<double> seidel(const matrix& matr, const std::vector<double>& b) {
    auto size = matr.size();
    AtomicsVector<double> x(size);
    std::vector<bool> status(size, false);
    std::atomic<std::size_t> count = size;
    bool is_over = false;
    double eps = 1e-3;
    auto calculate = [&](std::size_t start, std::size_t finish) -> void {
        while(!is_over) {
            for(std::size_t i = start; i < finish; ++i) {
                double sum = b[i];
                for(std::size_t j = 0; j < size; ++j) {
                    if(i != j) {
                        sum = sum - matr[i][j] * x[j];
                    }
                }
                double old_value = x[i];
                x[i] = (sum / matr[i][i]);
                if(!status[i] && abs(old_value - x[i]) < eps) {
                    status[i] = true;
                    count--;
                    if(count == 0) {
                        is_over = true;
                    }
                }
            }
        }
    };
    std::thread t1(calculate, 0, size / 2);
    std::thread t2(calculate, size / 2, size);
    t1.join();
    t2.join();
    return x;
}