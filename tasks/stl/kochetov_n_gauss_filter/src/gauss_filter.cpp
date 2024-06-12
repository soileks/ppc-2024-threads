// Copyright 2024 Kochetov Nikolay
#include "stl/kochetov_n_gauss_filter/include/gauss_filter.hpp"

#include <gtest/gtest.h>

#include <mutex>
#include <thread>
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

void GaussFilter::applyFilter(int startRow, int endRow) {
  for (int i = startRow; i < endRow; ++i) {
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
}

bool GaussFilter::run() {
  internal_order_test();

  const int numThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  int chunkSize = (x - 2) / numThreads;

  for (int i = 0; i < numThreads; ++i) {
    int startRow = 1 + i * chunkSize;
    int endRow = (i == numThreads - 1) ? x - 1 : startRow + chunkSize;
    threads.emplace_back(&GaussFilter::applyFilter, this, startRow, endRow);
  }

  for (auto& t : threads) {
    t.join();
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
