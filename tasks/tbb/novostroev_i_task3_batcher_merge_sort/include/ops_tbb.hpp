// Copyright 2024 Novostroev Ivan

#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

class BatcherMergeSortTBB : public ppc::core::Task {
 public:
  explicit BatcherMergeSortTBB(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> in, out, vec1, vec2;
};

inline std::vector<int> RandomVector(int size) {
  std::vector<int> vector(size);
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 900;
  }
  return vector;
}
