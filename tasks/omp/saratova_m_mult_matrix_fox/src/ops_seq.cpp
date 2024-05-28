// Copyright 2024 Saratova Marina
#include "omp/saratova_m_mult_matrix_fox/include/ops_seq.hpp"

#include <omp.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>

namespace saratova_omp {
  
void GenerateIdentityMatrix(double* matrix, int size, double scale) {
  std::fill(matrix, matrix + size * size, 0.0);
  for (int i = 0; i < size; ++i) {
    matrix[i * size + i] = scale;
  }
}

void CreateIdentityMatrix(double* matrix, int size, double scale) {
  std::fill(matrix, matrix + size * size, 0.0);
  for (int i = 0; i < size; ++i) {
    matrix[i * size + (size - i - 1)] = scale;
  }
}

void FillRandomValues(double* matrix, int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 99);
  for (int i = 0; i < size; ++i) {
    matrix[i] = dis(gen);
  }
}

bool SaratovaTaskSequential::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr && (taskData->outputs[0] != nullptr)) && (taskData->inputs[1] != nullptr) &&
         (taskData->outputs_count[0] == taskData->inputs_count[0]) &&
         (taskData->inputs_count[1] == taskData->inputs_count[0]);
}

bool SaratovaTaskSequential::pre_processing() {
  internal_order_test();
  matrixA = reinterpret_cast<double*>(taskData->inputs[0]);
  matrixB = reinterpret_cast<double*>(taskData->inputs[1]);
  matrixC = reinterpret_cast<double*>(taskData->outputs[0]);
  dimension = static_cast<size_t>(std::sqrt(taskData->inputs_count[0]));
  std::fill(matrixC, matrixC + dimension * dimension, 0.0);
  return true;
}

bool SaratovaTaskSequential::run() {
  internal_order_test();
  try {
    for (size_t i = 0; i < dimension; ++i) {
      for (size_t j = 0; j < dimension; ++j) {
        double sum = 0.0;
        for (size_t k = 0; k < dimension; ++k) {
          sum += matrixA[i * dimension + k] * matrixB[k * dimension + j];
        }
        matrixC[i * dimension + j] = sum;
      }
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return false;
  }
  return true;
}

bool SaratovaTaskSequential::post_processing() {
  internal_order_test();
  return true;
}

bool SaratovaTaskOmp::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr) && (taskData->inputs[1] != nullptr) && (taskData->outputs[0] != nullptr) &&
         (taskData->inputs_count[0] == taskData->inputs_count[1]) &&
         (taskData->inputs_count[0] == taskData->outputs_count[0]);
}

bool SaratovaTaskOmp::pre_processing() {
  internal_order_test();
  matrixA = reinterpret_cast<double*>(taskData->inputs[0]);
  matrixB = reinterpret_cast<double*>(taskData->inputs[1]);
  matrixC = reinterpret_cast<double*>(taskData->outputs[0]);
  dimension = static_cast<int>(std::sqrt(taskData->inputs_count[0]));
  std::fill(matrixC, matrixC + dimension * dimension, 0.0);
  return true;
}

bool SaratovaTaskOmp::run() {
  internal_order_test();
  try {
    const int numThreads = 2;
    const int blockSize = dimension / numThreads;
#pragma omp parallel for collapse(2) num_threads(numThreads* numThreads)
    for (int bi = 0; bi < dimension; bi += blockSize) {
      for (int bj = 0; bj < dimension; bj += blockSize) {
        for (int bk = 0; bk < dimension; bk += blockSize) {
          for (int i = bi; i < bi + blockSize; ++i) {
            for (int j = bj; j < bj + blockSize; ++j) {
              double sum = 0.0;
              for (int k = bk; k < bk + blockSize; ++k) {
                sum += matrixA[i * dimension + k] * matrixB[k * dimension + j];
              }
              matrixC[i * dimension + j] += sum;
            }
          }
        }
      }
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return false;
  }
  return true;
}

bool SaratovaTaskOmp::post_processing() {
  internal_order_test();
  return true;
}
}  // namespace saratova_omp
