// Copyright 2024 Saratova Marina
#include "omp/saratova_m_mult_matrix_fox/include/ops_seq.hpp"

#include <omp.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>

bool saratova_omp::SaratovaTaskSequential::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr) && (taskData->inputs[1] != nullptr) && (taskData->outputs[0] != nullptr) &&
         taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool saratova_omp::SaratovaTaskSequential::pre_processing() {
  internal_order_test();
  A = reinterpret_cast<double*>(taskData->inputs[0]);
  B = reinterpret_cast<double*>(taskData->inputs[1]);
  C = reinterpret_cast<double*>(taskData->outputs[0]);
  n = round(sqrt(taskData->inputs_count[0]));
  memset(C, 0, n * n * sizeof(*C));
  return true;
}

bool saratova_omp::SaratovaTaskSequential::run() {
  internal_order_test();
  try {
    double c = 0;
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < n; j++) {
        c = 0;
        for (size_t k = 0; k < n; k++) {
          c += A[i * n + k] * B[k * n + j];
        }
        C[i * n + j] = c;
      }
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

bool saratova_omp::SaratovaTaskSequential::post_processing() {
  internal_order_test();
  return true;
}

bool saratova_omp::SaratovaTaskOmp::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr) && (taskData->inputs[1] != nullptr) && (taskData->outputs[0] != nullptr) &&
         taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool saratova_omp::SaratovaTaskOmp::pre_processing() {
  internal_order_test();
  A = reinterpret_cast<double*>(taskData->inputs[0]);
  B = reinterpret_cast<double*>(taskData->inputs[1]);
  C = reinterpret_cast<double*>(taskData->outputs[0]);
  n = round(sqrt(taskData->inputs_count[0]));
  memset(C, 0, n * n * sizeof(*C));
  return true;
}

bool saratova_omp::SaratovaTaskOmp::run() {
  internal_order_test();
  try {
    int num = 2;
    int block_size = n / num;
    double* result_row;
    double* matrix_a_row;
    double* matrix_b_row;
    double tmp;
    int result_index;
    for (int l = 0; l < n; l += block_size) {
#pragma omp parallel for num_threads(num * num) private(result_index)
      for (int i = 0; i < n; i += block_size) {
        result_index = (i + l) % n;
#pragma omp parallel for num_threads(num) private(result_row, matrix_a_row, matrix_b_row, tmp)
        for (int k = 0; k < n; k += block_size) {
          for (int ii = 0; ii < block_size; ii++) {
            matrix_a_row = A + (i + ii) * n + (result_index);
            result_row = C + (i + ii) * n + (k);
            for (int jj = 0; jj < block_size; jj++) {
              matrix_b_row = B + (result_index)*n + (k + jj);
              tmp = 0;
              for (int kk = 0; kk < block_size; kk++) {
                tmp += matrix_a_row[kk] * matrix_b_row[kk * n];
              }
              result_row[jj] += tmp;
            }
          }
        }
      }
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

bool saratova_omp::SaratovaTaskOmp::post_processing() {
  internal_order_test();
  return true;
}

void saratova_omp::ScaledIdentityMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + (n - i - 1)] = 0;
    }
    matrix[i * n + (n - i - 1)] = k;
  }
}

void saratova_omp::IdentityMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + j] = 0;
    }
    matrix[i * n + i] = k;
  }
}

void saratova_omp::GenerateRandomValue(double* matrix, int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < sz; i++) {
    matrix[i] = gen() % 100;
  }
}
