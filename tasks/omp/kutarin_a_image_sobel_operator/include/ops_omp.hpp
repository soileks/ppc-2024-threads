// Copyright 2024 Kutarin Alexander
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

class SobelTaskParallel : public ppc::core::Task {
 public:
  explicit SobelTaskParallel(std::shared_ptr<ppc::core::TaskData> taskData_omp) : Task(std::move(taskData_omp)) {}
  bool validation() override;
  bool pre_processing() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> kernel_x = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

  std::vector<int> kernel_y = {1, 2, 1, 0, 0, 0, -1, -2, -1};

  std::vector<int> sourceImage;
  std::vector<int> resultImage;

  int width_;
  int height_;

  static int clamp(int value, int min, int max);
};
