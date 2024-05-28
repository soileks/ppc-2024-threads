// Copyright 2024 Bodrov Daniil
#pragma once

#include <complex>
#include <memory>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace bodrov_tbb {

struct SparseMatrixBodrovOMP {
  int Rows{};
  int Columns{};
  std::vector<std::complex<double>> Values{};
  std::vector<int> DataPointer{};
  std::vector<int> ColumnsIndexes{};
};

class SparseMatrixSolverBodrovOMP : public ppc::core::Task {
 public:
  explicit SparseMatrixSolverBodrovOMP(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  SparseMatrixBodrovOMP *MatrixA{}, *MatrixB{}, *Result{};
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
  SparseMatrixBodrovOMP *MatrixA{}, *MatrixB{}, *Result{};
};
}  // namespace bodrov_tbb