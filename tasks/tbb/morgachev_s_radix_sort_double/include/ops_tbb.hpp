// Copyright 2024 Morgachev Stepan
#pragma once

#include <tbb/tbb.h>

#include <vector>

#include "core/task/include/task.hpp"

namespace morgachev_tbb {

void countSort(double* in, double* out, int length, int exp);
void radixSortTBB(std::vector<double>& data, size_t dataSize);
std::vector<double> radixSortSeq(std::vector<double>& data);
std::vector<double> doubleMerge(const std::vector<double>& left, const std::vector<double>& right);

class RadixSortDoubleTBB : public ppc::core::Task {
 public:
  explicit RadixSortDoubleTBB(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> data;
  size_t dataSize;
};

}  // namespace morgachev_tbb
