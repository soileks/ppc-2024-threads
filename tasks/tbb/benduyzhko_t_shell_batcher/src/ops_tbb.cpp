// Copyright 2024 Benduyzhko Tatiana
#include "tbb/benduyzhko_t_shell_batcher/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

bool BenduyzhkoTBB::pre_processing() {
  internal_order_test();
  n = taskData->inputs_count[0];
  in_out = reinterpret_cast<int*>(taskData->outputs[0]);
  return true;
}

bool BenduyzhkoTBB::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->outputs_count[0] && taskData->inputs_count[0] != 0u;
}

namespace {
void shell(int* arr, int n);
}  // namespace

bool BenduyzhkoTBB::run() {
  internal_order_test();

  int p = n >> 1;
  tbb::parallel_for(0, 2, 1, [&](int i) { ::shell(in_out + i * p, p); });

  for (int k = p; k >= 1; k >>= 1) {
    tbb::parallel_for(k % p, n - k, 2 * k, [&](int j) {
      for (int i = 0; i < std::min<int>(k - 1, n - j - k - 1) + 1; ++i) {
        if ((i + j) / n == (i + j + k) / n) {
          if (in_out[i + j] > in_out[i + j + k]) {
            std::swap(in_out[i + j], in_out[i + j + k]);
          }
        }
      }
    });
  }

  return true;
}

bool BenduyzhkoTBB::post_processing() {
  internal_order_test();
  return true;
}

namespace {
void shell(int* arr, int n) {
  int tmp;
  int j;
  for (int gap = n >> 1; gap > 0; gap >>= 1) {
    for (int i = gap; i < n; ++i) {
      tmp = arr[i];
      for (j = i; j >= gap && arr[j - gap] > tmp; j -= gap) {
        arr[j] = arr[j - gap];
      }
      arr[j] = tmp;
    }
  }
}
}  // namespace

void BenduyzhkoTBB::get_random_numbers(int* arr, int n, int a, int b, int seed) {
  std::uniform_int_distribution<int> dist(a, b);
  std::random_device dev;
  std::mt19937 gen(seed != 0 ? seed : dev());
  for (int i = 0; i < n; i++) {
    arr[i] = dist(gen);
  }
}
