// Copyright 2024 Loginov Maxim

#pragma once

#include <tbb/tbb.h>

#include <algorithm>
#include <stack>
#include <vector>

#include "core/task/include/task.hpp"
#include "point.hpp"

namespace LoginovTbb {
class LoginovGrahAlgSequential : public ppc::core::Task {
 public:
  explicit LoginovGrahAlgSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<LoginovPoint> pointsArr, minConvexHull;
};

class LoginovGrahAlgTbbParallel : public ppc::core::Task {
 public:
  explicit LoginovGrahAlgTbbParallel(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<LoginovPoint> pointsArr, minConvexHull;
};

std::vector<LoginovPoint> LoginovRandomPoints(double leftBorder, double rightBorder, size_t size);
}  // namespace LoginovTbb
