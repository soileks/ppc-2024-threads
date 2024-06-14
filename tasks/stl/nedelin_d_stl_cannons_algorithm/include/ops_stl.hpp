//  Copyright 2024 Nedelin Dmitry
#pragma once

#ifndef TASKS_STL_NEDELIN_D_STL_CANNONS_ALGORITHM_INCLUDE_OPS_STL_HPP
#define TASKS_STL_NEDELIN_D_STL_CANNONS_ALGORITHM_INCLUDE_OPS_STL_HPP

#include <random>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class TestTBBSequentialNedelinCannon : public ppc::core::Task {
 public:
  explicit TestTBBSequentialNedelinCannon(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> A;
  std::vector<double> B;
  std::vector<double> res;
  int n = 0, m = 0;
};

std::vector<double> RndMatrix(int rows, int cols);

std::vector<double> multiplyMtrx(const std::vector<double>& A, const std::vector<double>& B, int rows_A, int col_B);

class TestTaskSTLParallelNedelinCannon : public ppc::core::Task {
 public:
  explicit TestTaskSTLParallelNedelinCannon(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> A;
  std::vector<double> B;
  std::vector<double> res;
  int n = 0, m = 0;
};

std::vector<double> cannonMtrxMultiplication_tbb(const std::vector<double>& A, const std::vector<double>& B, int n,
                                                 int m);

std::vector<double> cannonMtrxMultiplication(const std::vector<double>& A, const std::vector<double>& B, int n, int m);
#endif  // TASKS_STL_NEDELIN_D_STL_CANNONS_ALGORITHM_INCLUDE_OPS_STL_HPP