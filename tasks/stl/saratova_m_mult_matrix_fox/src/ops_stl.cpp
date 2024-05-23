// Copyright 2024 Saratova Marina
#include "stl/saratova_m_mult_matrix_fox/include/ops_stl.hpp"

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>

bool saratova_stl::SaratovaTaskSTL::validation() {
  internal_order_test();
  return (taskData->inputs[0] != nullptr) && (taskData->inputs[1] != nullptr) && (taskData->outputs[0] != nullptr) &&
          taskData->inputs_count[0] == taskData->inputs_count[1] &&
          taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool saratova_stl::SaratovaTaskSTL::pre_processing() {
  internal_order_test();
  A = reinterpret_cast<double*>(taskData->inputs[0]);
  B = reinterpret_cast<double*>(taskData->inputs[1]);
  C = reinterpret_cast<double*>(taskData->outputs[0]);
  n = round(sqrt(taskData->inputs_count[0]));
  memset(C, 0, n * n * sizeof(*C));
  return true;
}

void multiply_chunk(double* A, double* B, double* C, size_t n, size_t start, size_t end) {
  for (size_t i = start; i < end; i++) {
      for (size_t j = 0; j < n; j++) {
          double c = 0;
          for (size_t k = 0; k < n; k++) {
              c += A[i * n + k] * B[k * n + j];
          }
          C[i * n + j] = c;
      }
  }
}

bool saratova_stl::SaratovaTaskSTL::run() {
  internal_order_test();
  try {
      size_t num_threads = std::thread::hardware_concurrency();
      size_t chunk_size = n / num_threads;
      std::vector<std::thread> threads;

      for (size_t t = 0; t < num_threads; ++t) {
          size_t start = t * chunk_size;
          size_t end = (t == num_threads - 1) ? n : start + chunk_size;
          threads.emplace_back(multiply_chunk, A, B, C, n, start, end);
      }

      for (auto& thread : threads) {
          thread.join();
      }
  } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      return false;
  }
  return true;
}

bool saratova_stl::SaratovaTaskSTL::post_processing() {
  internal_order_test();
  return true;
}

void saratova_stl::ScaledIdentityMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
          matrix[i * n + (n - i - 1)] = 0;
      }
      matrix[i * n + (n - i - 1)] = k;
  }
}

void saratova_stl::IdentityMatrix(double* matrix, int n, double k) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
          matrix[i * n + j] = 0;
      }
      matrix[i * n + i] = k;
  }
}

void saratova_stl::GenerateRandomValue(double* matrix, int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < sz; i++) {
      matrix[i] = gen() % 100;
  }
}