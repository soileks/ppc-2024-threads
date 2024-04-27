// Copyright 2024 Chuvashov Andrey

#pragma once

#include<algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

class Chuvashov_OMPBatcherSort : public ppc::core::Task {
 public:
  explicit Chuvashov_OMPBatcherSort(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> input, output, arr1, arr2;
};

inline std::vector<int> ChuvashovOMP_GenerateVector(int size) {
  if (size < 1) throw -1;
  std::mt19937 gen;
  std::vector<int> vec(size);
  for (int i = 0; i < size; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}
