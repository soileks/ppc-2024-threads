// Copyright 2024 Isaev Dmitriy
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace IsaevTBB {

struct SparseMatrix {
  std::vector<double> values{};
  std::vector<int> column_indices{};
  std::vector<int> row_pointers{};

  int rows = 0;
  int columns = 0;
};

class SparseMultDoubleCRStbbSeq : public ppc::core::Task {
 public:
  explicit SparseMultDoubleCRStbbSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  SparseMatrix *A{}, *B{}, *C{};
};

class SparseMultDoubleCRStbbParallel : public ppc::core::Task {
 public:
  explicit SparseMultDoubleCRStbbParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  SparseMatrix *A{}, *B{}, *C{};
};

SparseMatrix getRandomMatrix(int _rows, int _columns, double chance, int seed);

}  // namespace IsaevTBB