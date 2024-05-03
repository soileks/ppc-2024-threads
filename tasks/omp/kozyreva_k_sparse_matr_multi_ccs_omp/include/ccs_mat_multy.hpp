// Copyright 2024 Kozyreva Ekaterina
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class SparseOmpMatrixMultiSequential : public ppc::core::Task {
 public:
  explicit SparseOmpMatrixMultiSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> values1{};
  std::vector<int> rows1{};
  std::vector<int> colPtr1{};
  int numRows1{};
  int numCols1{};
  std::vector<double> values2{};
  std::vector<int> rows2{};
  std::vector<int> colPtr2{};
  int numRows2{};
  int numCols2{};
  std::vector<double> values3{};
  std::vector<int> rows3{};
  std::vector<int> colPtr3{};
  int numRows3{};
  int numCols3{};
  double* result{};
};

class SparseOmpMatrixMultiParallel : public ppc::core::Task {
 public:
  explicit SparseOmpMatrixMultiParallel(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> values1{};
  std::vector<int> rows1{};
  std::vector<int> colPtr1{};
  int numRows1{};
  int numCols1{};
  std::vector<double> values2{};
  std::vector<int> rows2{};
  std::vector<int> colPtr2{};
  int numRows2{};
  int numCols2{};
  std::vector<double> values3{};
  std::vector<int> rows3{};
  std::vector<int> colPtr3{};
  int numRows3{};
  int numCols3{};
  double* result{};
};
