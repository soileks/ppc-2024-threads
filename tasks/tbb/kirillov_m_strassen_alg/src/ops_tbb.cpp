// Copyright 2024 Kirillov Maxim

#include "tbb/kirillov_m_strassen_alg/include/ops_tbb.hpp"

#include <tbb/parallel_invoke.h>

#include <cmath>
#include <random>
using namespace kirillov_tbb;

std::vector<double> kirillov_tbb::strassen(const std::vector<double>& A, const std::vector<double>& B, int n) {
  if ((n == 0) || ((n & (n - 1)) != 0)) {
    throw std::invalid_argument("Matrix size is not 2^n");
  }

  if (n <= 2) {
    return mul(A, B, 2);
  }

  int half = n / 2;

  std::vector<double> A11(half * half);
  std::vector<double> A12(half * half);
  std::vector<double> A21(half * half);
  std::vector<double> A22(half * half);

  std::vector<double> B11(half * half);
  std::vector<double> B12(half * half);
  std::vector<double> B21(half * half);
  std::vector<double> B22(half * half);

  splitMatrix(A, A11, A12, A21, A22);
  splitMatrix(B, B11, B12, B21, B22);
  std::vector<double> p1;
  std::vector<double> p2;
  std::vector<double> p3;
  std::vector<double> p4;
  std::vector<double> p5;
  std::vector<double> p6;
  std::vector<double> p7;
  tbb::parallel_invoke(
      [&] { p1 = strassen(add(A11, A22), add(B11, B22), half); }, [&] { p2 = strassen(add(A21, A22), B11, half); },
      [&] { p3 = strassen(A11, sub(B12, B22), half); }, [&] { p4 = strassen(A22, sub(B21, B11), half); },
      [&] { p5 = strassen(add(A11, A12), B22, half); }, [&] { p6 = strassen(sub(A21, A11), add(B11, B12), half); },
      [&] { p7 = strassen(sub(A12, A22), add(B21, B22), half); });
  std::vector<double> C11 = add(add(p1, p4), sub(p7, p5));
  std::vector<double> C12 = add(p3, p5);
  std::vector<double> C21 = add(p2, p4);
  std::vector<double> C22 = add(sub(p1, p2), add(p3, p6));

  return joinMatrices(C11, C12, C21, C22, n);
}

std::vector<double> kirillov_tbb::joinMatrices(const std::vector<double>& A11, const std::vector<double>& A12,
                                               const std::vector<double>& A21, const std::vector<double>& A22, int n) {
  int half = n / 2;
  std::vector<double> A(n * n, 0.0);
  for (int i = 0; i < half; i++) {
    for (int j = 0; j < half; j++) {
      A[i * n + j] = A11[i * half + j];
      A[i * n + j + half] = A12[i * half + j];
      A[(i + half) * n + j] = A21[i * half + j];
      A[(i + half) * n + j + half] = A22[i * half + j];
    }
  }
  return A;
}

void kirillov_tbb::splitMatrix(const std::vector<double>& A, std::vector<double>& A11, std::vector<double>& A12,
                               std::vector<double>& A21, std::vector<double>& A22) {
  int half = std::sqrt(A.size()) / 2;
  for (int i = 0; i < half; i++) {
    for (int j = 0; j < half; j++) {
      A11[i * half + j] = A[i * half * 2 + j];
      A12[i * half + j] = A[i * half * 2 + j + half];
      A21[i * half + j] = A[(i + half) * half * 2 + j];
      A22[i * half + j] = A[(i + half) * half * 2 + j + half];
    }
  }
}

std::vector<double> kirillov_tbb::add(const std::vector<double>& A, const std::vector<double>& B) {
  int n = A.size();
  std::vector<double> C(n);
  for (int i = 0; i < n; i++) {
    C[i] = A[i] + B[i];
  }
  return C;
}

std::vector<double> kirillov_tbb::sub(const std::vector<double>& A, const std::vector<double>& B) {
  int n = A.size();
  std::vector<double> C(n);
  for (int i = 0; i < n; i++) {
    C[i] = A[i] - B[i];
  }
  return C;
}

std::vector<double> kirillov_tbb::mul(const std::vector<double>& A, const std::vector<double>& B, int n) {
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

std::vector<double> kirillov_tbb::generateRandomMatrix(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(1.0, 5.0);
  std::vector<double> matrix(n * n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      matrix[i * n + j] = dis(gen);
    }
  }
  return matrix;
}

bool StrassenMatrixMultParallelTBB::pre_processing() {
  internal_order_test();
  A.resize(taskData->inputs_count[0]);
  B.resize(taskData->inputs_count[1]);

  n = *reinterpret_cast<int*>(taskData->inputs[2]);

  auto* aPtr = reinterpret_cast<double*>(taskData->inputs[0]);
  auto* bPtr = reinterpret_cast<double*>(taskData->inputs[1]);

  for (unsigned int i = 0; i < taskData->inputs_count[0]; i++) {
    A[i] = aPtr[i];
    B[i] = bPtr[i];
  }

  return true;
}

bool StrassenMatrixMultParallelTBB::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool StrassenMatrixMultParallelTBB::run() {
  internal_order_test();
  C = strassen(A, B, n);
  return true;
}

bool StrassenMatrixMultParallelTBB::post_processing() {
  internal_order_test();
  std::copy(C.begin(), C.end(), reinterpret_cast<double*>(taskData->outputs[0]));
  return true;
}
