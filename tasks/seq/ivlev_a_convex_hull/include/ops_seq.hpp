// Copyright 2024 Ivlev Alexander
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class ConvexHullSequential : public ppc::core::Task {
 public:
  explicit ConvexHullSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<std::pair<size_t, size_t>> sizes;
  std::vector<std::vector<std::pair<size_t, size_t>>> components;
  std::vector<std::vector<int>> images;
  std::vector<std::vector<std::pair<size_t, size_t>>> results;

  static std::vector<std::pair<size_t, size_t>> ToComponents(const std::vector<int>& image_,
                                                             std::pair<size_t, size_t> size_);
  std::vector<int> ToImage(const std::vector<std::pair<size_t, size_t>>& component_, std::pair<size_t, size_t> size_);
};
