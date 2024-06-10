// Copyright 2024 Bakhtiarov Alexander
#pragma once

#include <vector>
#include <memory>
#include "core/task/include/task.hpp"

class SparseTBBMatrixMulti : public ppc::core::Task {
 public:
  explicit SparseTBBMatrixMulti(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool run() override = 0; // Pure virtual
  bool post_processing() override;
  bool validation() override;

 protected:
  struct Matrix {
    std::vector<double> values;
    std::vector<int> rows;
    std::vector<int> colPtr;
    int numRows;
    int numCols;
  };

  Matrix matrix1;
  Matrix matrix2;
  Matrix result;

  void construct_ccs(Matrix& matrix, const double* data, int numRows, int numCols);
  void internal_order_test();
};

class SparseTBBMatrixMultiSequential : public SparseTBBMatrixMulti {
 public:
  using SparseTBBMatrixMulti::SparseTBBMatrixMulti;
  bool run() override;
};

class SparseTBBMatrixMultiParallel : public SparseTBBMatrixMulti {
 public:
  using SparseTBBMatrixMulti::SparseTBBMatrixMulti;
  bool run() override;
};
