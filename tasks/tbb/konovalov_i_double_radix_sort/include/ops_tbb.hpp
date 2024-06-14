// Copyright 2024 Konovalov Igor
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class RadixSortTBBTaskParallel : public ppc::core::Task {
 public:
  explicit RadixSortTBBTaskParallel(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  static void radix_sort_seq(double* input_, size_t input_size, std::vector<double>* ordered_local);
  size_t input_size;
  double* input_;
  std::vector<double> ordered[sizeof(double) * 256];
};