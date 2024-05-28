// Copyright 2024 Shmelev Ivan
#pragma once
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace shmelev_tbb {

std::vector<int> create_random_sequence(int size, int min, int max);
bool is_sorted(std::vector<int> input);

class ShmelevTaskSequential : public ppc::core::Task {
 public:
  explicit ShmelevTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> sequence;
  std::vector<int> result;
  void batcherMergeSort(int left, int right);
  void shellSort();
  void mergeSequences(int left, int middle, int right);
};

class ShmelevTaskTbb : public ppc::core::Task {
 public:
  explicit ShmelevTaskTbb(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> sequence;
  std::vector<int> result;
  void batcherMergeSort(int left, int right);
  void shellSort();
  void mergeSequences(int left, int middle, int right);
};

}  // namespace shmelev_tbb