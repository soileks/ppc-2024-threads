// Copyright 2024 Bodrov Daniil
#pragma once

#include <complex>
#include <memory>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace bodrov_tbb {

struct SparseMatrixBodrovOMP {
  int n_rows{};
  int n_cols{};
  std::vector<std::complex<double>> non_zero_values{};
  std::vector<int> pointer{};
  std::vector<int> col_indexes{};
};

class SparseMatrixSolverBodrovOMP : public ppc::core::Task {
 public:
  explicit SparseMatrixSolverBodrovOMP(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  SparseMatrixBodrovOMP *A_M{}, *B_M{}, *Result{};
};

class SparseMatrixSolverBodrovOMPParallel : public ppc::core::Task {
 public:
  explicit SparseMatrixSolverBodrovOMPParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  SparseMatrixBodrovOMP *A_M{}, *B_M{}, *Result{};
};
}  // namespace bodrov_omp