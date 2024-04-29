// Copyright 2024 Vasilev Ivan
#pragma once
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

class VasilevTaskOmp : public ppc::core::Task {
 public:
  explicit VasilevTaskOmp(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  static std::vector<int> generate_random_vector(int size, int min, int max);
  static bool R_sorted(std::vector<int> input);

 private:
  std::vector<int> input_;
  std::vector<int> res;
  void merge(int l, int m, int r);
  void shellSort(std::vector<int>& arr, int n);
  void  shell_sort_batcher(std::vector<int>& arr, int n);
  // static void shellort_parallel(std::vector<int>& input);
  void batcherMerge(int l, int r);
};