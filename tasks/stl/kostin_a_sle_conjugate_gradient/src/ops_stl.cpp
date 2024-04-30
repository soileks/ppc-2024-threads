// Copyright 2024 Kostin Artem
#include "stl/kostin_a_sle_conjugate_gradient/include/ops_stl.hpp"

#include <random>
#include <thread>

using namespace std::chrono_literals;

namespace KostinArtemSTL {
std::vector<double> dense_matrix_vector_multiply(const std::vector<double>& A, int n, const std::vector<double>& x) {
  std::vector<double> result(n, 0.0);

  const auto num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(num_threads);
  int chunk_size = n / num_threads;

  for (unsigned int thr_ind = 0; thr_ind < num_threads; ++thr_ind) {
    int start = thr_ind * chunk_size;
    int end = (thr_ind == num_threads - 1) ? n : (thr_ind + 1) * chunk_size;
    threads[thr_ind] = std::thread([&, start, end]() {
      for (int i = start; i < end; ++i) {
        for (int j = 0; j < n; ++j) {
          result[i] += A[i * n + j] * x[j];
        }
      }
    });
  }
  for (auto& thread : threads) thread.join();

  return result;
}

double dot_product(const std::vector<double>& a, const std::vector<double>& b) {
  std::atomic<double> result = 0.0;

  const auto num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(num_threads);
  int chunk_size = a.size() / num_threads;

  for (unsigned int thr_ind = 0; thr_ind < num_threads; ++thr_ind) {
    int start = thr_ind * chunk_size;
    int end = (thr_ind == num_threads - 1) ? a.size() : (thr_ind + 1) * chunk_size;
    threads[thr_ind] = std::thread([&, start, end]() {
      double local_result = 0.0;
      for (int i = start; i < end; ++i) {
        local_result += a[i] * b[i];
      }
      result += local_result;
    });
  }
  for (auto& thread : threads) thread.join();

  return result;
}

std::vector<double> conjugate_gradient(const std::vector<double>& A, int n, const std::vector<double>& b,
                                       double tolerance) {
  std::vector<double> x(n, 0.0);
  std::vector<double> r = b;
  std::vector<double> p = r;
  std::vector<double> r_prev = b;

  const auto num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(num_threads);
  int chunk_size = n / num_threads;

  while (true) {
    std::vector<double> Ap = dense_matrix_vector_multiply(A, n, p);
    double alpha = dot_product(r, r) / dot_product(Ap, p);

    for (unsigned int thr_ind = 0; thr_ind < num_threads; ++thr_ind) {
      int start = thr_ind * chunk_size;
      int end = (thr_ind == num_threads - 1) ? n : (thr_ind + 1) * chunk_size;
      threads[thr_ind] = std::thread([&, start, end]() {
        for (int i = start; i < end; ++i) {
          x[i] += alpha * p[i];
          r[i] = r_prev[i] - alpha * Ap[i];
        }
      });
    }
    for (auto& thread : threads) thread.join();

    if (sqrt(dot_product(r, r)) < tolerance) {
      break;
    }

    double beta = dot_product(r, r) / dot_product(r_prev, r_prev);
    for (unsigned int thr_ind = 0; thr_ind < num_threads; ++thr_ind) {
      int start = thr_ind * chunk_size;
      int end = (thr_ind == num_threads - 1) ? n : (thr_ind + 1) * chunk_size;
      threads[thr_ind] = std::thread([&, start, end]() {
        for (int i = start; i < end; ++i) {
          p[i] = r[i] + beta * p[i];
        }
      });
    }
    for (auto& thread : threads) thread.join();

    r_prev = r;
  }

  return x;
}

std::vector<double> generateSPDMatrix(int size, int max_value) {
  std::vector<double> A(size * size);
  std::mt19937 gen(4041);
  for (int i = 0; i < size; ++i) {
    for (int j = i; j < size; ++j) {
      A[i * size + j] = static_cast<double>(gen() % max_value + 1);
    }
  }

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < i; ++j) {
      A[i * size + j] = A[j * size + i];
    }
  }
  return A;
}

std::vector<double> generatePDVector(int size, int max_value) {
  std::vector<double> vec(size);
  std::mt19937 gen(4140);
  for (int i = 0; i < size; ++i) {
    vec[i] = static_cast<double>(gen() % max_value + 1);
  }
  return vec;
}

bool check_solution(const std::vector<double>& A, int n, const std::vector<double>& b, const std::vector<double>& x,
                    double tolerance) {
  std::vector<double> Ax = dense_matrix_vector_multiply(A, n, x);

  for (int i = 0; i < n; ++i) {
    if (std::abs(Ax[i] - b[i]) > tolerance) {
      return false;
    }
  }
  return true;
}

bool ConjugateGradientMethodSTL::pre_processing() {
  internal_order_test();
  // Init value for input and output
  A = std::vector<double>(taskData->inputs_count[0]);
  // for (unsigned int i = 0; i < taskData->inputs_count[0]; i++) {
  //   A[i] = reinterpret_cast<double*>(taskData->inputs[0])[i];
  // }
  std::copy(reinterpret_cast<double*>(taskData->inputs[0]),
            reinterpret_cast<double*>(taskData->inputs[0]) + taskData->inputs_count[0], A.begin());

  b = std::vector<double>(taskData->inputs_count[1]);
  // for (unsigned int i = 0; i < taskData->inputs_count[1]; i++) {
  //   b[i] = reinterpret_cast<double*>(taskData->inputs[1])[i];
  // }
  std::copy(reinterpret_cast<double*>(taskData->inputs[1]),
            reinterpret_cast<double*>(taskData->inputs[1]) + taskData->inputs_count[1], b.begin());

  size = *reinterpret_cast<int*>(taskData->inputs[2]);
  x = std::vector<double>(size, 0);
  return true;
}

bool ConjugateGradientMethodSTL::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] == taskData->inputs_count[1] * taskData->inputs_count[1] &&
         taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool ConjugateGradientMethodSTL::run() {
  internal_order_test();
  x = conjugate_gradient(A, size, b, 1e-6);
  return true;
}

bool ConjugateGradientMethodSTL::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < x.size(); i++) {
    reinterpret_cast<double*>(taskData->outputs[0])[i] = x[i];
  }
  return true;
}
}  // namespace KostinArtemSTL
