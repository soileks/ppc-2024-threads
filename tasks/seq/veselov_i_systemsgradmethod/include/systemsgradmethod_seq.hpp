// Copyright 2024 Veselov Ilya
#pragma once

#include <vector>
#include "core/task/include/task.hpp"

class SystemsGradMethodSeq : public ppc::core::Task {
    std::vector<double> A;
    std::vector<double> b;
    std::vector<double> x;
    int rows;

    std::vector<double> SLEgradSolver(const std::vector<double>& A, const std::vector<double>& b, int n, double tol = 1e-6);
    double dotProduct(const std::vector<double>& a, const std::vector<double>& b);
    std::vector<double> matrixVectorProduct(const std::vector<double>& A, const std::vector<double>& x, int n);
    void normalize(std::vector<double>& A);
public:
    explicit SystemsGradMethodSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
    bool pre_processing() override;
    bool validation() override;
    bool run() override;
    bool post_processing() override;
};

bool checkSolution(const std::vector<double>& A, const std::vector<double>& b, const std::vector<double>& x, double tol = 1e-6);
std::vector<double> genRandomVector(int size, int minVal, int maxVal);