// Copyright 2024 Saratova Marina
#include "tbb/saratova_m_mult_matrix_fox/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>

bool saratova_tbb::SaratovaTaskTbb::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr) && (taskData->inputs[1] != nullptr) && (taskData->outputs[0] != nullptr) &&
         taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool saratova_tbb::SaratovaTaskTbb::pre_processing() {
  internal_order_test();
  A = reinterpret_cast<double*>(taskData->inputs[0]);
  B = reinterpret_cast<double*>(taskData->inputs[1]);
  C = reinterpret_cast<double*>(taskData->outputs[0]);
  n = round(sqrt(taskData->inputs_count[0]));
  memset(C, 0, n * n * sizeof(*C));
  return true;
}

bool saratova_tbb::SaratovaTaskTbb::run() {
  internal_order_test();
  try {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n, 1), [&](const tbb::blocked_range<size_t>& range) {
      for (size_t i = range.begin(); i != range.end(); ++i) {
        for (size_t j = 0; j < n; ++j) {
          double c = 0.0;
          for (size_t k = 0; k < n; ++k) {
            c += A[i * n + k] * B[k * n + j];
          }
          C[i * n + j] = c;
        }
      }
    });
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

bool saratova_tbb::SaratovaTaskTbb::post_processing() {
  internal_order_test();
  return true;
}

void saratova_tbb::ScaledIdentityMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + (n - i - 1)] = 0;
    }
    matrix[i * n + (n - i - 1)] = k;
  }
}

void saratova_tbb::IdentityMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + j] = 0;
    }
    matrix[i * n + i] = k;
  }
}

void saratova_tbb::GenerateRandomValue(double* matrix, int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < sz; i++) {
    matrix[i] = gen() % 100;
  }
}