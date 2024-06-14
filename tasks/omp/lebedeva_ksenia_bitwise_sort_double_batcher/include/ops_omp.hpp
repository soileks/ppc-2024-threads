// Copyright 2024 Lebedeva Ksenia
#pragma once

#include <omp.h>

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

#include "core/task/include/task.hpp"

namespace LebedevaKOmp {

class RadixSortDoubleWithBatcherSequential : public ppc::core::Task {
 public:
  explicit RadixSortDoubleWithBatcherSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> array;
  std::vector<double> result;
};

class RadixSortDoubleWithBatcherOmpParallel : public ppc::core::Task {
 public:
  explicit RadixSortDoubleWithBatcherOmpParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> array;
  std::vector<double> result;
};

std::vector<double> batchersMergeSeq(std::vector<std::vector<double>>& subvectors);
void partSortSeq(std::vector<std::vector<double>>& parts, std::vector<double>& side);
std::vector<double> radixSortWithBatcherSeq(std::vector<double> vec);

std::vector<double> batchersMergeOmp(std::vector<std::vector<double>>& subvectors);
void partSortOmp(std::vector<std::vector<double>>& parts, std::vector<double>& side);
std::vector<double> radixSortWithBatcherOmp(std::vector<double> vec);

std::vector<double> randomVector(int sizeVec, double minValue, double maxValue);
}  // namespace LebedevaKOmp