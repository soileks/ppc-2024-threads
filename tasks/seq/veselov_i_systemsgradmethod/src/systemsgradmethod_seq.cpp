// Copyright 2024 Veselov Ilya

#include "seq/veselov_i_systemsgradmethod/include/systemsgradmethod_seq.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>

using namespace std::chrono_literals;

bool SystemsGradMethodSeq::pre_processing() {
  try {
    internal_order_test();
    A = std::vector<double>(taskData->inputs_count[0]);
    std::copy(reinterpret_cast<double *>(taskData->inputs[0]),
              reinterpret_cast<double *>(taskData->inputs[0]) + taskData->inputs_count[0], A.begin());
    b = std::vector<double>(taskData->inputs_count[1]);
    std::copy(reinterpret_cast<double *>(taskData->inputs[1]),
              reinterpret_cast<double *>(taskData->inputs[1]) + taskData->inputs_count[1], b.begin());
    rows = *reinterpret_cast<int *>(taskData->inputs[2]);
    x = std::vector<double>(rows, 0.0);
  } catch (...) {
    return false;
  }
  return true;
}

bool SystemsGradMethodSeq::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->inputs_count[1] * taskData->inputs_count[1] &&
         taskData->inputs_count[1] == taskData->outputs_count[0];
}

double tol = 1e-6;

bool SystemsGradMethodSeq::run() {
  try {
    internal_order_test();
    x = SLEgradSolver(A, b, rows);
    std::this_thread::sleep_for(30ms);
  } catch (...) {
    return false;
  }
  return true;
}

bool SystemsGradMethodSeq::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < x.size(); ++i) {
    reinterpret_cast<double *>(taskData->outputs[0])[i] = x[i];
  }
  return true;
}

void SystemsGradMethodSeq::normalize(std::vector<double> &matrix) {
  double max_val = matrix[0];
  for (size_t i = 1; i < rows * rows; ++i) {
    if (matrix[i] > max_val) {
      max_val = matrix[i];
    }
  }
  for (size_t i = 0; i < rows * rows; ++i) {
    matrix[i] /= max_val;
  }
}

bool checkSolution(const std::vector<double> &A, const std::vector<double> &b, const std::vector<double> &x,
                   double tol) {
  int n = b.size();
  std::vector<double> Ax(n, 0.0);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      Ax[i] += A[i * n + j] * x[j];
    }
  }
  for (int i = 0; i < n; ++i) {
    if (std::abs(Ax[i] - b[i]) > tol) {
      return false;
    }
  }
  return true;
}

double SystemsGradMethodSeq::dotProduct(const std::vector<double> &a, const std::vector<double> &b) {
  double result = 0.0;
  for (size_t i = 0; i < a.size(); ++i) {
    result += a[i] * b[i];
  }
  return result;
}

std::vector<double> SystemsGradMethodSeq::matrixVectorProduct(const std::vector<double> &A,
                                                              const std::vector<double> &x, int n) {
  std::vector<double> result(n, 0.0);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      result[i] += A[i * n + j] * x[j];
    }
  }
  return result;
}

std::vector<double> genRandomVector(int size, int maxVal) {
  std::vector<double> res(size);
  std::mt19937 gen(4140);
  for (int i = 0; i < size; ++i) {
    res[i] = static_cast<double>(gen() % maxVal + 1);
  }
  return res;
}

std::vector<double> genRandomMatrix(int size, int maxVal) {
  std::vector<double> matrix(size * size);
  std::mt19937 gen(4041);
  for (int i = 0; i < size; ++i) {
    for (int j = i; j < size; ++j) {
      matrix[i * size + j] = static_cast<double>(gen() % maxVal + 1);
    }
  }
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < i; ++j) {
      matrix[i * size + j] = matrix[j * size + i];
    }
  }
  return matrix;
}

std::vector<double> SystemsGradMethodSeq::SLEgradSolver(const std::vector<double> &A, const std::vector<double> &b,
                                                        int n, double tol) {
  std::vector<double> res(n, 0.0);
  std::vector<double> r = b;
  std::vector<double> p = r;
  std::vector<double> r_old = b;

  while (true) {
    std::vector<double> Ap = matrixVectorProduct(A, p, n);
    double alpha = dotProduct(r, r) / dotProduct(Ap, p);
    for (size_t i = 0; i < res.size(); ++i) {
      res[i] += alpha * p[i];
    }
    for (size_t i = 0; i < r.size(); ++i) {
      r[i] = r_old[i] - alpha * Ap[i];
    }
    if (sqrt(dotProduct(r, r)) < tol) {
      break;
    }
    double beta = dotProduct(r, r) / dotProduct(r_old, r_old);
    for (size_t i = 0; i < p.size(); ++i) {
      p[i] = r[i] + beta * p[i];
    }
    r_old = r;
  }
  return res;
}
