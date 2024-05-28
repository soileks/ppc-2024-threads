// Copyright 2024 Saratova Marina
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace saratova_omp {

void GenerateIdentityMatrix(double* matrix, int size, double scale = 1.0);
void CreateIdentityMatrix(double* matrix, int size, double scale = 1.0);
void FillRandomValues(double* matrix, int size);

class SaratovaTaskSequential : public ppc::core::Task {
 public:
  explicit SaratovaTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double *matrixA{nullptr}, *matrixB{nullptr}, *matrixC{nullptr};
  size_t dimension{};
};

class SaratovaTaskOmp : public ppc::core::Task {
 public:
  explicit SaratovaTaskOmp(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double *matrixA{nullptr}, *matrixB{nullptr}, *matrixC{nullptr};
  int dimension{};
};

}  // namespace saratova_omp
