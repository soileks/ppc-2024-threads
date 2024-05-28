// Copyright 2024 Saratova Marina
#include "stl/saratova_m_mult_matrix_fox/include/ops_stl.hpp"

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>

void saratova_stl::GenerateIdentityMatrix(double* matrix, int size, double scale) {
  std::fill(matrix, matrix + size * size, 0.0);
  for (int i = 0; i < size; ++i) {
    matrix[i * size + i] = scale;
  }
}

void saratova_stl::CreateIdentityMatrix(double* matrix, int size, double scale) {
  std::fill(matrix, matrix + size * size, 0.0);
  for (int i = 0; i < size; ++i) {
    matrix[i * size + (size - i - 1)] = scale;
  }
}

void saratova_stl::FillRandomValues(double* matrix, int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 99);
  for (int i = 0; i < size; ++i) {
    matrix[i] = dis(gen);
  }
}

bool saratova_stl::SaratovaTaskSequential::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr) && (taskData->inputs[1] != nullptr) &&
         (taskData->outputs_count[0] == taskData->inputs_count[0]) &&
         (taskData->inputs_count[1] == taskData->inputs_count[0]);
}

bool saratova_stl::SaratovaTaskSequential::pre_processing() {
  internal_order_test();
  matrixA = reinterpret_cast<double*>(taskData->inputs[0]);
  matrixB = reinterpret_cast<double*>(taskData->inputs[1]);
  matrixC = reinterpret_cast<double*>(taskData->outputs[0]);
  dimension = static_cast<size_t>(std::sqrt(taskData->inputs_count[0]));
  std::fill(matrixC, matrixC + dimension * dimension, 0.0);
  return true;
}

bool saratova_stl::SaratovaTaskSequential::run() {
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

bool saratova_stl::SaratovaTaskSequential::post_processing() {
  internal_order_test();
  return true;
}

bool saratova_stl::SaratovaTaskSTL::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr) && (taskData->inputs[1] != nullptr) &&
         (taskData->outputs_count[0] == taskData->inputs_count[0]) &&
         (taskData->inputs_count[1] == taskData->inputs_count[0]);
}

bool saratova_stl::SaratovaTaskSTL::pre_processing() {
  internal_order_test();
  matrixA = reinterpret_cast<double*>(taskData->inputs[0]);
  matrixB = reinterpret_cast<double*>(taskData->inputs[1]);
  matrixC = reinterpret_cast<double*>(taskData->outputs[0]);
  dimension = static_cast<size_t>(std::sqrt(taskData->inputs_count[0]));
  std::fill(matrixC, matrixC + dimension * dimension, 0.0);
  return true;
}

bool saratova_stl::SaratovaTaskSTL::run() {
  internal_order_test();
  try {
    unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    auto compute_partial_matrix = [&](size_t start_row, size_t end_row) {
      for (size_t i = start_row; i < end_row; ++i) {
        for (size_t j = 0; j < dimension; ++j) {
          double sum = 0.0;
          for (size_t k = 0; k < dimension; ++k) {
            sum += matrixA[i * dimension + k] * matrixB[k * dimension + j];
          }
          matrixC[i * dimension + j] = sum;
        }
      }
    };

    size_t rows_per_thread = dimension / num_threads;
    size_t start_row = 0;
    size_t end_row = 0;
    for (unsigned int i = 0; i < num_threads - 1; ++i) {
      end_row = start_row + rows_per_thread;
      threads.emplace_back(compute_partial_matrix, start_row, end_row);
      start_row = end_row;
    }
    threads.emplace_back(compute_partial_matrix, start_row, dimension);

    for (auto& thread : threads) {
      thread.join();
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return false;
  }
  return true;
}

bool saratova_stl::SaratovaTaskSTL::post_processing() {
  internal_order_test();
  return true;
}
