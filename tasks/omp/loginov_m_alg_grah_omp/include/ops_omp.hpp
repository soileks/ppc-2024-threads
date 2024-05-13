// Copyright 2024 Loginov Maxim

#pragma once

#include <omp.h>

#include <algorithm>
#include <stack>
#include <vector>

#include "core/task/include/task.hpp"
#include "point.hpp"

namespace LoginovOmp {
class LoginovGrahAlgSequential : public ppc::core::Task {
 public:
  explicit LoginovGrahAlgSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<LoginovPoint> pointsArr, minConvexHull;
};

class LoginovGrahAlgOmpParallel : public ppc::core::Task {
 public:
  explicit LoginovGrahAlgOmpParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<LoginovPoint> pointsArr, minConvexHull;
};

std::vector<LoginovPoint> LoginovRandomPoints(double leftBorder, double rightBorder, size_t size);
}  // namespace SavotinaOmp