// Copyright 2024 Savchuk Anton
#pragma once

#include <vector>

#include "core/task/include/task.hpp"

class SavchukCRSMatMultOMPSequential : public ppc::core::Task {
 public:
  explicit SavchukCRSMatMultOMPSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> values1{};
  std::vector<int> rowPtr1{};
  std::vector<int> colPtr1{};
  int numRows1{};
  int numCols1{};
  std::vector<double> values2{};
  std::vector<int> rowPtr2{};
  std::vector<int> colPtr2{};
  int numRows2{};
  int numCols2{};
  double* result{};
};

class SavchukCRSMatMultOMPParallel : public ppc::core::Task {
 public:
  explicit SavchukCRSMatMultOMPParallel(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> values1{};
  std::vector<int> rowPtr1{};
  std::vector<int> colPtr1{};
  int numRows1{};
  int numCols1{};
  std::vector<double> values2{};
  std::vector<int> rowPtr2{};
  std::vector<int> colPtr2{};
  int numRows2{};
  int numCols2{};
  double* result{};
};