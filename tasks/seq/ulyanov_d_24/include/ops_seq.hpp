// Copyright 2023 Ulyanov Daniil
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class Pixel {
 public:
  Pixel() {
    r = 0;
    g = 0;
    b = 0;
  };

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class FilterGaussHorizontalSequentialUlyanov : public ppc::core::Task {
 public:
  explicit FilterGaussHorizontalSequentialUlyanov(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<Pixel> inputImage;
  std::vector<Pixel> resultImage;
  std::vector<float> kernel;
  int width, height;
};