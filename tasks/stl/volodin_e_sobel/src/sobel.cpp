// Copyright 2024 Volodin Evgeniy
#include "stl/volodin_e_sobel/include/sobel.hpp"

#include <cmath>
#include <functional>
#include <iostream>

bool SobelTaskStlVolodin::validation() {
  internal_order_test();
  return (taskData->inputs_count.size() == 2) && (taskData->outputs_count.size() == 2);
}

bool SobelTaskStlVolodin::pre_processing() {
  internal_order_test();
  try {
    width_ = taskData->inputs_count[0];
    height_ = taskData->inputs_count[1];
    sourceImage.reserve(width_ * height_);
    resultImage.reserve(width_ * height_);

    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numThreads);

    int elementsPerThread = (width_ * height_) / numThreads;
    for (int t = 0; t < numThreads; ++t) {
      int startIdx = t * elementsPerThread;
      int endIdx = (t == numThreads - 1) ? (width_ * height_) : startIdx + elementsPerThread;
      threads[t] = std::thread([this, startIdx, endIdx, capture0 = reinterpret_cast<int *>(taskData->inputs[0]),
                                capture1 = sourceImage.data()] { copyRange(startIdx, endIdx, capture0, capture1); });
    }

    for (auto &thread : threads) {
      thread.join();
    }

  } catch (...) {
    return false;
  }
  return true;
}

bool SobelTaskStlVolodin::run() {
  internal_order_test();
  try {
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numThreads);

    int rowsPerThread = height_ / numThreads;
    for (int t = 0; t < numThreads; ++t) {
      int startRow = t * rowsPerThread;
      int endRow = (t == numThreads - 1) ? height_ : startRow + rowsPerThread;
      threads[t] = std::thread([startRow, endRow] { processPartOfImageVolodin(startRow, endRow); });
    }

    for (auto &thread : threads) {
      thread.join();
    }
    return true;
  } catch (...) {
    return false;
  }
}

bool SobelTaskStlVolodin::post_processing() {
  internal_order_test();
  try {
    taskData->outputs_count[0] = width_;
    taskData->outputs_count[1] = height_;

    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numThreads);

    int elementsPerThread = (width_ * height_) / numThreads;
    for (int t = 0; t < numThreads; ++t) {
      int startIdx = t * elementsPerThread;
      int endIdx = (t == numThreads - 1) ? (width_ * height_) : startIdx + elementsPerThread;
      threads[t] = std::thread([startIdx, endIdx, capture0 = resultImage.data(),
                                capture1 = reinterpret_cast<int *>(taskData->outputs[0])] {
        copyRange(startIdx, endIdx, capture0, capture1);
      });
    }

    for (auto &thread : threads) {
      thread.join();
    }
  } catch (...) {
    return false;
  }
  return true;
}

void SobelTaskStlVolodin::processPartOfImageVolodin(int startRow, int endRow) {
  for (int j = startRow; j < endRow; ++j) {
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
}

void SobelTaskStlVolodin::copyRange(int startIdx, int endIdx, const int *source, int *destination) {
  for (int i = startIdx; i < endIdx; ++i) {
    destination[i] = source[i];
  }
}

int SobelTaskStlVolodin::clamp(int value, int min, int max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}