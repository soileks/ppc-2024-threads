// Copyright 2024 Kutarin Alexander
#pragma once

#include <tbb/tbb.h>

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class KutarinASobel : public ppc::core::Task {
 public:
  explicit KutarinASobel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

  int width_{0};
  int height_{0};

  std::vector<int> sourceImage;
  std::vector<int> resultImage;
  std::vector<int> kernel_x = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  std::vector<int> kernel_y = {1, 2, 1, 0, 0, 0, -1, -2, -1};

  static void generateSaltAndPepperNoise(std::vector<int>& image, int height, int width, float noise_ratio);
  static int clamp(int value, int min, int max);
};
