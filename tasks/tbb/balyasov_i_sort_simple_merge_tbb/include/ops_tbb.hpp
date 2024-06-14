// Copyright 2024 Brlyasov Ivan
#pragma once

#include <tbb/tbb.h>

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class RadixSortClassTaskTbb : public ppc::core::Task {
 public:
  explicit RadixSortClassTaskTbb(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> VectorSort;
  int SizeVector = 0;
};

class RadixSortClassClist {
 public:
  mutable std::vector<int> VectorResult;
  RadixSortClassClist() = default;
  RadixSortClassClist(const RadixSortClassClist&, tbb::split) {}

  void operator()(const tbb::blocked_range<std::vector<int>::iterator>& r) {
    std::vector<int> temp(r.begin(), r.end());
    temp = radixSort(temp);
    VectorResult = mergeVector(&temp, &VectorResult);
  }

  void join(RadixSortClassClist& rhs) { VectorResult = mergeVector(&rhs.VectorResult, &VectorResult); }

  static std::vector<int> radixSort(std::vector<int> vec);
  static std::vector<int> mergeVector(std::vector<int>* vec1, std::vector<int>* vec2);
};
