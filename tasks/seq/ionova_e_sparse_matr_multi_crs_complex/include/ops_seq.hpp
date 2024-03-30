// Copyright 2023 Nesterov Alexander
#pragma once

#include <vector>

#include "core/task/include/task.hpp"

struct Complex {
  double real;
  double imag;
};

class SparseMatrixComplexMultiSequential : public ppc::core::Task {
 public:
  explicit SparseMatrixComplexMultiSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<Complex> values1{};
  std::vector<int> rowPtr1{};
  std::vector<int> colPtr1{};
  int numRows1{};
  int numCols1{};
  std::vector<Complex> values2{};
  std::vector<int> rowPtr2{};
  std::vector<int> colPtr2{};
  int numRows2{};
  int numCols2{};
  std::vector<Complex> values3{};
  std::vector<int> rowPtr3{};
  std::vector<int> colPtr3{};
  int numRows3{};
  int numCols3{};
  Complex* result{};
};
