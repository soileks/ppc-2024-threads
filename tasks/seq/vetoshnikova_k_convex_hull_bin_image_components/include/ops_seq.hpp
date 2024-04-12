// Copyright 2024 Vetoshnikova Ekaterina
#pragma once

#include <algorithm>
#include <stack>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class ConstructingConvexHullSeq : public ppc::core::Task {
 public:
  explicit ConstructingConvexHullSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  int h{};
  int w{};
  int numComponents;
  std::vector<std::vector<int>> img = {};
  std::vector<int> hull = {};

  void marking—omponent();
  void convexHull(int label);
};

struct Point {
  uint32_t x, y;
}
