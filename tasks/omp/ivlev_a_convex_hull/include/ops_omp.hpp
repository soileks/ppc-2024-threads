// Copyright 2024 Ivlev Alexander
#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class ConvexHullOMPTaskSequential : public ppc::core::Task {
 public:
  explicit ConvexHullOMPTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<std::pair<size_t, size_t>> sizes;
  std::vector<std::vector<std::pair<size_t, size_t>>> components;
  std::vector<std::vector<std::pair<size_t, size_t>>> results;

  static std::vector<std::pair<size_t, size_t>> Convex_Hull(
    const std::vector<std::pair<size_t, size_t>>& component_);
};

class ConvexHullOMPTaskParallel : public ppc::core::Task {
 public:
  explicit ConvexHullOMPTaskParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<std::pair<size_t, size_t>> sizes;
  std::vector<std::vector<std::pair<size_t, size_t>>> components;
  std::vector<std::vector<std::pair<size_t, size_t>>> results;

  static std::vector<std::pair<size_t, size_t>> Convex_Hull(
    const std::vector<std::pair<size_t, size_t>>& component_);
};
