// Copyright 2024 Ivlev Alexander
#pragma once

#include <algorithm>
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
  std::vector<std::vector<size_t>> images;
  std::vector<std::vector<std::pair<size_t, size_t>>> results;

  std::vector<std::pair<size_t, size_t>> ToComponents(const std::vector<size_t>& image_, std::pair<size_t, size_t> size_);
  std::vector<std::pair<size_t, size_t>> ToComponents(const std::vector<size_t>& image_,
                                                      std::pair<size_t, size_t> size_);
  std::vector<size_t> ToImage(const std::vector<std::pair<size_t, size_t>>& component_,
                              std::pair<size_t, size_t> size_);
};
