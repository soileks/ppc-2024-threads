// Copyright 2024 Ryabkov Vladislav

#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "core/task/include/task.hpp"

class SeqBatcher : public ppc::core::Task {
 public:
  explicit SeqBatcher(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> inv, result, a1, a2;
};

inline std::vector<int> GetRandomVector(int size) {
  std::random_device rand;
  std::uniform_int_distribution<int> unif(0, 1000000);
  std::vector<int> vect(size);
  for (int i = 0; i < size; i++) {
    vect[i] = unif(rand);
  }

  return vect;
}
