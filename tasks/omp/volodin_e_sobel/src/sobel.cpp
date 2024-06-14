// Copyright 2024 Volodin Evgeniy
#include "omp/volodin_e_sobel/include/sobel.hpp"

#include <cmath>
#include <iostream>

bool SobelTaskOMP::validation() {
  internal_order_test();
  return (taskData->inputs_count.size() == 2) && (taskData->outputs_count.size() == 2);
}

bool SobelTaskOMP::pre_processing() {
  internal_order_test();
  try {
    width_ = taskData->inputs_count[0];
    height_ = taskData->inputs_count[1];
    sourceImage.reserve(width_ * height_);
    resultImage.reserve(width_ * height_);
#pragma omp parallel for
    for (int i = 0; i < width_ * height_; ++i) {
      sourceImage[i] = reinterpret_cast<int*>(taskData->inputs[0])[i];
    }
  } catch (...) {
    return false;
  }
  return true;
}

bool SobelTaskOMP::run() {
  internal_order_test();
  try {
#pragma omp parallel for
    for (int j = 0; j < height_; ++j) {
      for (int i = 0; i < width_; ++i) {
        int resultX = 0;
        int resultY = 0;

        resultX += sourceImage[clamp((j - 1), 0, height_ - 1) * width_ + clamp((i - 1), 0, width_ - 1)] * kernel_x[0] +
                   sourceImage[clamp((j - 1), 0, height_ - 1) * width_ + clamp(i, 0, width_ - 1)] * kernel_x[1] +
                   sourceImage[clamp((j - 1), 0, height_ - 1) * width_ + clamp((i + 1), 0, width_ - 1)] * kernel_x[2] +
                   sourceImage[clamp(j, 0, height_ - 1) * width_ + clamp((i - 1), 0, width_ - 1)] * kernel_x[3] +
                   sourceImage[clamp(j, 0, height_ - 1) * width_ + clamp(i, 0, width_ - 1)] * kernel_x[4] +
                   sourceImage[clamp(j, 0, height_ - 1) * width_ + clamp((i + 1), 0, width_ - 1)] * kernel_x[5] +
                   sourceImage[clamp((j + 1), 0, height_ - 1) * width_ + clamp((i - 1), 0, width_ - 1)] * kernel_x[6] +
                   sourceImage[clamp((j + 1), 0, height_ - 1) * width_ + clamp(i, 0, width_ - 1)] * kernel_x[7] +
                   sourceImage[clamp((j + 1), 0, height_ - 1) * width_ + clamp((i + 1), 0, width_ - 1)] * kernel_x[8];

        resultY += sourceImage[clamp((j - 1), 0, height_ - 1) * width_ + clamp((i - 1), 0, width_ - 1)] * kernel_y[0] +
                   sourceImage[clamp((j - 1), 0, height_ - 1) * width_ + clamp(i, 0, width_ - 1)] * kernel_y[1] +
                   sourceImage[clamp((j - 1), 0, height_ - 1) * width_ + clamp((i + 1), 0, width_ - 1)] * kernel_y[2] +
                   sourceImage[clamp(j, 0, height_ - 1) * width_ + clamp((i - 1), 0, width_ - 1)] * kernel_y[3] +
                   sourceImage[clamp(j, 0, height_ - 1) * width_ + clamp(i, 0, width_ - 1)] * kernel_y[4] +
                   sourceImage[clamp(j, 0, height_ - 1) * width_ + clamp((i + 1), 0, width_ - 1)] * kernel_y[5] +
                   sourceImage[clamp((j + 1), 0, height_ - 1) * width_ + clamp((i - 1), 0, width_ - 1)] * kernel_y[6] +
                   sourceImage[clamp((j + 1), 0, height_ - 1) * width_ + clamp(i, 0, width_ - 1)] * kernel_y[7] +
                   sourceImage[clamp((j + 1), 0, height_ - 1) * width_ + clamp((i + 1), 0, width_ - 1)] * kernel_y[8];

        resultImage[j * width_ + i] = clamp((int)sqrt(resultX * resultX + resultY * resultY), 0, 255);
      }
    }
    return true;
  } catch (...) {
    return false;
  }
}

bool SobelTaskOMP::post_processing() {
  internal_order_test();
  taskData->outputs_count[0] = width_;
  taskData->outputs_count[1] = height_;
#pragma omp parallel for
  for (int i = 0; i < width_ * height_; ++i) {
    reinterpret_cast<int*>(taskData->outputs[0])[i] = resultImage[i];
  }
  return true;
}

int SobelTaskOMP::clamp(int value, int min, int max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}