// Copyright 2024 Lapin Dmitriy
#include "seq/lapin_d_image_filter_block/include/ops_seq.hpp"
#include <cmath>
#include <iostream>
#include <vector>

int Clamp(int value, int min, int max) {
  if (value < min) {
    return min;
  }
  if (value > max) {
    return max;
  }
  return value;
}

std::vector<std::vector<double>> create2DFilter(int n, double sigma) {
  double PI = 3.141592653;
  std::vector<double> temp(3);
  std::vector<std::vector<double>> filter(3, temp);
  double coeff = 1.0 / (2.0 * PI * sigma * sigma);
  int middle = n / 2;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      double x = double(j - middle);
      double y = double(i - middle);
      filter[i][j] = coeff * exp(-(x * x + y * y) / (2.0 * sigma * sigma));
    }
  }
  return filter;
}

bool BlockFilterSeq::pre_processing() {
  internal_order_test();
  height = taskData->inputs_count[0];
  width = taskData->inputs_count[1];
  mas_in =
      reinterpret_cast<std::vector<std::vector<int>> *>(taskData->inputs[0]);
  mas_out =
      reinterpret_cast<std::vector<std::vector<int>> *>(taskData->outputs[0]);
  return true;
}

bool BlockFilterSeq::validation() {
  internal_order_test();
  return taskData->inputs_count[0] > 0 && taskData->inputs_count[1] > 0;
}

bool BlockFilterSeq::run() {
  internal_order_test();
  std::vector<std::vector<double>> kernel = create2DFilter(3, 1);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      double result = 0;
      for (int l = -1; l <= 1; l++) {
        for (int k = -1; k <= 1; k++) {
          int idX = Clamp(i + k, 0, height - 1);
          int idY = Clamp(j + l, 0, width - 1);
          result += (*mas_in)[idX][idY] * kernel[k + 1][l + 1];
        }
      }
      (*mas_out)[i][j] = Clamp((int)result, 0, 255);
    }
  }
  return true;
}

bool BlockFilterSeq::post_processing() {
  internal_order_test();
  return true;
}