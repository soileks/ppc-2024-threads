// Copyright 2024 Kanakov Roman
#pragma once
#include <string>
#include <vector>
#include "core\task\include\task.hpp"

namespace kanakov_omp {
class HoareSortSequential : public ppc::core::Task {
 public:
  explicit HoareSortSequential(std::shared_ptr<ppc::core::TaskData> taskData_): Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  static void HoareSortSeq(std::vector<int> &arr, size_t l, size_t r);

 private:
  std::vector<int> array{};
};

class HoareSortOMP : public ppc::core::Task {
 public:
  explicit HoareSortOMP(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  static void HoareSortParallel(std::vector<int> &arr, size_t l, size_t r);

 private:
  std::vector<int> array{};
};
}  // namespace kanakov_omp