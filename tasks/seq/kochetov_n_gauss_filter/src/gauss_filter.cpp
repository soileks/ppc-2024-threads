// Copyright 2024 Kochetov Nikolay
#include "seq/kochetov_n_gauss_filter/include/gauss_filter.hpp"

#include <gtest/gtest.h>

#include <random>
#include <vector>

int* GaussFilter::getImgId(int a, int b) { return &input[a * x + b]; }

int* GaussFilter::getResId(int a, int b) { return &result[a * x + b]; }

int GaussFilter::clampData(int value, int min, int max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

bool GaussFilter::pre_processing() {
  internal_order_test();

  x = taskData->inputs_count[0];
  y = taskData->inputs_count[1];

  input = reinterpret_cast<int*>(taskData->inputs[0]);
  result = reinterpret_cast<int*>(taskData->outputs[0]);

  return true;
}

bool GaussFilter::run() {
  internal_order_test();

  for (int i = 1; i < x - 1; ++i) {
    for (int j = 1; j < y - 1; ++j) {
      int sum = 0;
      for (int ki = -1; ki <= 1; ++ki) {
        for (int kj = -1; kj <= 1; ++kj) {
          sum += *getImgId(ki + i, kj + j) * kernel[ki + 1][kj + 1];
        }
      }
      *getResId(i, j) = (int)(sum / 16);
    }
  }
  return true;
}

bool GaussFilter::post_processing() {
  internal_order_test();

  for (int i = 0; i < x; ++i) {
    for (int j = 0; j < y; ++j) {
      *getResId(i, j) = clampData(*getResId(i, j), 0, 255);
    }
  }
  return true;
}

bool GaussFilter::validation() {
  internal_order_test();

  return !taskData->inputs.empty() && !taskData->outputs.empty() && !taskData->inputs_count.empty() &&
          !taskData->outputs_count.empty() && taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr &&
          taskData->outputs_count[1] == taskData->inputs_count[1] && taskData->outputs_count[0] >= 3 &&
          taskData->outputs_count[1] >= 3;
}
