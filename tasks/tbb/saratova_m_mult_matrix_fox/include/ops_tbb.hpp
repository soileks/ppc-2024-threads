// Copyright 2024 Saratova Marina
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace saratova_tbb {

void GenerateIdentityMatrix(double *matrix, int size, double scale = 1.0);
void CreateIdentityMatrix(double *matrix, int size, double scale = 1.0);
void FillRandomValues(double *matrix, int size);

class SaratovaTaskSequential : public ppc::core::Task {
 private:
  size_t dimension{};
  double *matrixA{nullptr};
  double *matrixB{nullptr};
  double *matrixC{nullptr};

 public:
  explicit SaratovaTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
};

class SaratovaTaskTbb : public ppc::core::Task {
 private:
  size_t dimension{};
  double *matrixA{nullptr};
  double *matrixB{nullptr};
  double *matrixC{nullptr};

 public:
  explicit SaratovaTaskTbb(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
};
}  // namespace saratova_tbb