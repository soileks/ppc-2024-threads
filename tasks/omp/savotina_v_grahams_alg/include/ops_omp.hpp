// Copyright 2024 Savotina Valeria

#pragma once

#include <omp.h>

#include <cfloat>
#include <stack>
#include <vector>

#include "core/task/include/task.hpp"
#include "point.hpp"

namespace SavotinaOmp {
class SavotinaGrahamsAlgorithmSequential : public ppc::core::Task {
 public:
  explicit SavotinaGrahamsAlgorithmSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<SavotinaPoint> pointsArr, minConvexHull;
};

class SavotinaGrahamsAlgorithmOmpParallel : public ppc::core::Task {
 public:
  explicit SavotinaGrahamsAlgorithmOmpParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<SavotinaPoint> pointsArr, minConvexHull;
};

std::vector<SavotinaPoint> SavotinaRandomPoints(double leftBorder, double rightBorder, size_t size);
}  // namespace SavotinaOmp
