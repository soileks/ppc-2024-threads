// Copyright 2024 Kistrimova Ekaterina
#pragma once

#include <vector>

#include "core/task/include/task.hpp"
namespace KistrimovaETaskSeq {

using Point = std::pair<double, double>;

class GrahamAlgTask : public ppc::core::Task {
 public:
  explicit GrahamAlgTask(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<Point> points;
  std::vector<Point> finalHull;
};
}  // namespace KistrimovaETaskSeq
