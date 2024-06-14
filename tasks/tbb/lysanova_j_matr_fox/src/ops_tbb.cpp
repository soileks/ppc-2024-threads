// Copyright 2024 Lysanova Julia
#include "tbb/lysanova_j_matr_fox/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>

bool LysanovaTaskOmp::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr) && (taskData->inputs[1] != nullptr) && (taskData->outputs[0] != nullptr) &&
         taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool LysanovaTaskOmp::pre_processing() {
  internal_order_test();
  A = reinterpret_cast<double*>(taskData->inputs[0]);
  B = reinterpret_cast<double*>(taskData->inputs[1]);
  C = reinterpret_cast<double*>(taskData->outputs[0]);
  n = round(sqrt(taskData->inputs_count[0]));
  memset(C, 0, n * n * sizeof(*C));
  return true;
}

bool LysanovaTaskOmp::run() {
  internal_order_test();
  try {
    int q = 2;
    int t = n / q;
    for (int l = 0; l < n; l += t) {
      tbb::parallel_for(0, n, t, [&](int i) {
        int j = (i + l) % n;
        tbb::parallel_for(0, n, t, [&](int k) {
          double* c;
          double* a;
          double* b;
          double tmp;
          for (int ii = 0; ii < t; ii++) {
            a = A + (i + ii) * n + (j);
            c = C + (i + ii) * n + (k);
            for (int jj = 0; jj < t; jj++) {
              b = B + (j)*n + (k + jj);
              tmp = 0;
              for (int kk = 0; kk < t; kk++) {
                tmp += a[kk] * b[kk * n];
              }
              c[jj] += tmp;
            }
          }
        });
      });
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

bool LysanovaTaskOmp::post_processing() {
  internal_order_test();
  return true;
}

namespace lysanova_j_matr_fox_tbb {
void RandomFillMatrix(double* matrix, int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < sz; i++) {
    matrix[i] = gen() % 100;
  }
}

void EMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + j] = 0;
    }
    matrix[i * n + i] = k;
  }
}
void EsdMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + (n - i - 1)] = 0;
    }
    matrix[i * n + (n - i - 1)] = k;
  }
}
}  // namespace lysanova_j_matr_fox_tbb
