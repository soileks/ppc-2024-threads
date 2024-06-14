// Copyright 2024 Kurdina Julia
#ifndef TASKS_EXAMPLES_TEST_TBB_OPS_TBB_H_
#define TASKS_EXAMPLES_TEST_TBB_OPS_TBB_H_

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class HorizontalSplitTBB_seq : public ppc::core::Task {
 public:
  explicit HorizontalSplitTBB_seq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  uint8_t NewColor(int i, int j);

 private:
  std::vector<uint8_t> input_;
  std::vector<uint8_t> image;
  std::vector<uint8_t> result_im;
  std::vector<uint8_t> gauss_kernel;
  int n, m;
  int ker_size = 3;
};

class HorizontalSplitTBB_parallel : public ppc::core::Task {
 public:
  explicit HorizontalSplitTBB_parallel(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  uint8_t NewColor(int i, int j);

 private:
  std::vector<uint8_t> input_;
  std::vector<uint8_t> image;
  std::vector<uint8_t> result_im;
  std::vector<uint8_t> gauss_kernel;
  int n, m;
  int ker_size = 3;
};
#endif  // TASKS_EXAMPLES_TEST_TBB_OPS_TBB_H_
