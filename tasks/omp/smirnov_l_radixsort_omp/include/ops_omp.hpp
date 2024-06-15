// Copyright 2024 Smirnov Leonid
#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

std::vector<int> getRandomVectorSmirn(int length);

class RadixSortOMPSequential : public ppc::core::Task {
 public:
  explicit RadixSortOMPSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> workVector;
};

class RadixSortOMPParallelS : public ppc::core::Task {
 public:
  explicit RadixSortOMPParallelS(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> workVector;
};
