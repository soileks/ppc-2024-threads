// Copyright 2024 Bakhtiarov Alexander
#pragma once

#include <tbb/tbb.h>

#include <vector>

#include "core/task/include/task.hpp"

class SparseMatrixMultiTBB {
 public:
  explicit SparseMatrixMultiTBB(std::shared_ptr<ppc::core::TaskData> taskData);
  bool pre_processing();
  bool run();
  bool post_processing();

 private:
  ppc::core::TaskData* taskData;
  std::vector<double> values1, values2, values3;
  std::vector<int> rows1, rows2, rows3;
  std::vector<int> colPtr1, colPtr2, colPtr3;
  double* result;
  int numRows1, numCols1, numRows2, numCols2, numRows3, numCols3;

  void internal_order_test();
};
