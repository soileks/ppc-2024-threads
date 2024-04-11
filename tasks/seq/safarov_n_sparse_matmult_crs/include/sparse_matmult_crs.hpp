// Copyright 2024 Safarov Nurlan
#pragma once

#ifndef TASKS_SEQ_SAFAROV_N_SPARSE_MATMULT_CRS_INCLUDE_SPARSE_MATMULT_CRS_HPP_
#define TASKS_SEQ_SAFAROV_N_SPARSE_MATMULT_CRS_INCLUDE_SPARSE_MATMULT_CRS_HPP_

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

class SparseMatrixMultiplicationCRS : public ppc::core::Task {
 public:
  explicit SparseMatrixMultiplicationCRS(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool validation() override;
  bool pre_processing() override;
  bool run() override;
  bool post_processing() override;

 private:
  SparseMatrixCRS *X{}, *Y{}, *Z{};
};

#endif  // TASKS_SEQ_SAFAROV_N_SPARSE_MATMULT_CRS_INCLUDE_SPARSE_MATMULT_CRS_HPP_
