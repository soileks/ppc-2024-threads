// Copyright 2024 Lebedeva Ksenia

#pragma once
#include <tbb/tbb.h>

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

#include "core/task/include/task.hpp"

namespace LebedevaKTbb {

std::vector<double> batchersMergeSeq(std::vector<std::vector<double>>& subVec);
std::vector<double> batchersMergeTbb(std::vector<std::vector<double>>& subVec);

void sortPartTbb(std::vector<std::vector<double>>& parts, std::vector<double>& vec);
void sortPartSeq(std::vector<std::vector<double>>& parts, std::vector<double>& vec);

std::vector<double> radixSortWithBatcherSeq(std::vector<double> vec);
std::vector<double> radixSortWithBatcherTbb(std::vector<double> vec);

std::vector<double> randomVector(int size, double min, double max);

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

class RadixSortDoubleWithBatcherTbbParallel : public ppc::core::Task {
 public:
  explicit RadixSortDoubleWithBatcherTbbParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> array;
  std::vector<double> result;
};
}  // namespace LebedevaKTbb
