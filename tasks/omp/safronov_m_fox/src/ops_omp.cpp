// Copyright 2024 Safronov Mikhail

#include "omp/safronov_m_fox/include/ops_omp.hpp"

#include <omp.h>

#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

using std::cout;
using std::endl;

bool SafronovSeqFoxAlgTaskOMP::validation() {
  internal_order_test();
  size_t input_count = taskData->inputs_count[0];
  n = static_cast<size_t>(round(sqrt(input_count)));
  bool valid = (taskData->inputs[0] != nullptr) && (taskData->inputs[1] != nullptr) &&
               (taskData->outputs[0] != nullptr) && (input_count == taskData->inputs_count[1]) &&
               (input_count == taskData->outputs_count[0]) && (n * n == input_count);
  if (!valid) {
    std::cerr << "Validation failed: " << std::endl;
    std::cerr << "input_count: " << input_count << std::endl;
    std::cerr << "inputs[0] != nullptr: " << (taskData->inputs[0] != nullptr) << std::endl;
    std::cerr << "inputs[1] != nullptr: " << (taskData->inputs[1] != nullptr) << std::endl;
    std::cerr << "outputs[0] != nullptr: " << (taskData->outputs[0] != nullptr) << std::endl;
    std::cerr << "inputs_count[0] == inputs_count[1]: " << (input_count == taskData->inputs_count[1]) << std::endl;
    std::cerr << "inputs_count[0] == outputs_count[0]: " << (input_count == taskData->outputs_count[0]) << std::endl;
    std::cerr << "n * n == input_count: " << (n * n == input_count) << std::endl;
  }
  return valid;
}

bool SafronovSeqFoxAlgTaskOMP::pre_processing() {
  internal_order_test();
  int num_threads = 2;
  size_t matrix_size = taskData->inputs_count[0];
  if (matrix_size % num_threads != 0) {
    std::cerr << "Matrix size must be divisible by the number of threads\n";
    return false;
  }
  A = reinterpret_cast<double*>(taskData->inputs[0]);
  B = reinterpret_cast<double*>(taskData->inputs[1]);
  C = reinterpret_cast<double*>(taskData->outputs[0]);
  return true;
}

bool SafronovSeqFoxAlgTaskOMP::run() {
  internal_order_test();
  try {
    int num_threads = 2;
    size_t block_size = n / num_threads;

    double* result_row;
    double* matrix_a_block_row;
    double* matrix_b_block_row;

    double tmp;

    size_t result_row_index;

    int n_int = static_cast<int>(n);
    int block_size_int = static_cast<int>(block_size);

#pragma omp parallel for num_threads(num_threads) private(matrix_a_block_row, matrix_b_block_row, result_row, tmp, \
                                                              result_row_index)
    for (int i = 0; i < n_int; i += block_size_int) {
      for (int k = 0; k < n_int; k += block_size_int) {
        for (int l = 0; l < n_int; l += block_size_int) {
          result_row_index = static_cast<size_t>((i + l) % n_int);
          for (int ii = 0; ii < block_size_int; ii++) {
            matrix_a_block_row = A + (i + ii) * n + result_row_index;
            result_row = C + (i + ii) * n + k;
            for (int jj = 0; jj < block_size_int; jj++) {
              matrix_b_block_row = B + result_row_index * n + (k + jj);
              tmp = 0;
              for (int kk = 0; kk < block_size_int; kk++) {
                tmp += matrix_a_block_row[kk] * matrix_b_block_row[kk * n];
              }
#pragma omp atomic
              result_row[jj] += tmp;
            }
          }
        }
      }
    }
  } catch (const std::exception& e) {
    cout << e.what() << endl;
    return false;
  }
  return true;
}

bool SafronovSeqFoxAlgTaskOMP::post_processing() {
  internal_order_test();
  return true;
}

void GetRandomValue(double* m, int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < size; i++) {
    m[i] = gen() % 100;
  }
}

void identityMatrix(double* m, int n, double k) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      m[i * n + j] = 0;
    }
    m[i * n + i] = k;
  }
}

void ModifidentityMatrix(double* m, int n, double k) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      m[i * n + (n - i - 1)] = 0;
    }
    m[i * n + (n - i - 1)] = k;
  }
}

std::vector<double> mulSafronov(const std::vector<double>& A, const std::vector<double>& B, int n) {
  if (n == 0) {
    return std::vector<double>();
  }
  std::vector<double> C(n * n, 0.0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[k * n + j];
      }
    }
  }
  return C;
}
