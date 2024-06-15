// Copyright 2024 Kistrimova Ekaterina
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

struct point {
  double x, y;
  bool operator==(const point& other) const { return (x == other.x && y == other.y); }
};

class GrahamAlgTask : public ppc::core::Task {
 public:
  explicit GrahamAlgTask(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<point> input{};
  std::vector<point> output{};
};

std::vector<point> graham(std::vector<point> points);