// Copyright 2024 Safarov Nurlan
#pragma once

#ifndef TASKS_OMP_SAFAROV_N_SPARSE_MATMULT_CRS_INCLUDE_SPARSE_MATMULT_CRS_OMP_HPP_
#define TASKS_OMP_SAFAROV_N_SPARSE_MATMULT_CRS_INCLUDE_SPARSE_MATMULT_CRS_OMP_HPP_

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

struct SparseMatrixCRS {
  int numberOfRows = 0;
  int numberOfColumns = 0;

  std::vector<int> pointers{};
  std::vector<int> columnIndexes{};
  std::vector<double> values{};
};

class SparseMatrixMultiplicationCRS_OMP : public ppc::core::Task {
 public:
  explicit SparseMatrixMultiplicationCRS_OMP(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool validation() override;
  bool pre_processing() override;
  bool run() override;
  bool post_processing() override;

 private:
  SparseMatrixCRS *X{}, *Y{}, *Z{};
};

#endif  // TASKS_OMP_SAFAROV_N_SPARSE_MATMULT_CRS_INCLUDE_SPARSE_MATMULT_CRS_OMP_HPP_
