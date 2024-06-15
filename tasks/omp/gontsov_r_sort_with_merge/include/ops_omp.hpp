// Copyright 2023 Nesterov Alexander
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

std::vector<int> getRandomVector2(int sz);

class RadixSortOMPSequential_G : public ppc::core::Task {
 public:
  explicit RadixSortOMPSequential_G(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  std::vector<int> input_;
};

class RadixSortOMPParallel : public ppc::core::Task {
 public:
  explicit RadixSortOMPParallel(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> input_;
};
