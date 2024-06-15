// Copyright 2024 Smirnov Leonid

#pragma once

#include <tbb/tbb.h>

#include <memory>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

std::vector<int> getRandomVectorSmirnov(int length);

class RadixSortTBBMethod : public ppc::core::Task {
 public:
  explicit RadixSortTBBMethod(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> workVector;
};

class RadixSortTBBBody {
 public:
  mutable std::vector<int> workVectorResult;
  static std::vector<int> radixSortSmirnov(std::vector<int> vector);
  static std::vector<int> mergeListsSmirnov(const std::vector<int>& firstVector, const std::vector<int>& secondVector);

  RadixSortTBBBody() = default;
  RadixSortTBBBody(const RadixSortTBBBody&, tbb::split) {}

  void operator()(const tbb::blocked_range<std::vector<int>::iterator>& r) {
    std::vector<int> tmp(r.begin(), r.end());
    tmp = radixSortSmirnov(tmp);
    workVectorResult = mergeListsSmirnov(tmp, workVectorResult);
  }

  void join(RadixSortTBBBody& rhs) { workVectorResult = mergeListsSmirnov(rhs.workVectorResult, workVectorResult); }
};
