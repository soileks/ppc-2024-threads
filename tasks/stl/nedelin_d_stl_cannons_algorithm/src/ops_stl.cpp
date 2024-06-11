// Copyright 2024 Nedelin Dmitry
#include "stl/nedelin_d_stl_cannons_algorithm/include/ops_stl.hpp"

#include <algorithm>
#include <iostream>
#include<thread>

#undef min

std::vector<double> cannonMtrxMultiplication(const std::vector<double>& A, const std::vector<double>& B, int n, int m) {
  int SizeBlock = std::min(n, m);

  std::vector<double> mtrx_C(n * m, 0.0);

  if (n == 0 || m == 0) {
    return std::vector<double>();
  }

  for (int i = 0; i < n; i += SizeBlock) {
    for (int j = 0; j < m; j += SizeBlock) {
      for (int k = 0; k < m; k += SizeBlock) {
        int i_end = std::min(i + SizeBlock, n);
        int j_end = std::min(j + SizeBlock, m);
        int k_end = std::min(k + SizeBlock, m);

        for (int ii = i; ii < i_end; ++ii) {
          for (int kk = k; kk < k_end; ++kk) {
            double A_ik = A[ii * m + kk];
            for (int jj = j; jj < j_end; ++jj) {
              mtrx_C[ii * m + jj] += A_ik * B[kk * m + jj];
            }
          }
        }
      }
    }
  }

  return mtrx_C;
}

void BlocksMultiply(const std::vector<double>& A, const std::vector<double>& B, std::vector<double>& C, int n, int m,
                    int _SizeBlock, int _iCount, int _jCount, int _kCount) {
  for (int i = _iCount; i < std::min(_iCount + _SizeBlock, n); ++i) {
    for (int j = _jCount; j < std::min(_jCount + _SizeBlock, m); ++j) {
      for (int k = _kCount; k < std::min(_kCount + _SizeBlock, m); ++k) {
        C[i * m + j] += A[i * m + k] * B[k * m + j];
      }
    }
  }
}

std::vector<double> cannonMtrxMultiplication_tbb(const std::vector<double>& A, const std::vector<double>& B, int n,
                                                 int m) {
  int SizeBlock = std::min(n, m);

  std::vector<double> mtrx_C(n * m, 0.0);

  if (n == 0 || m == 0) {
    return std::vector<double>();
  }

  int Threads_num = std::thread::hardware_concurrency();
  if (Threads_num == 0) {
    Threads_num = 1;
  }

  std::vector<std::thread> threads(Threads_num);
  int SizeBlockForThread = (n + Threads_num - 1) / Threads_num;

  for (int thrd = 0; thrd < Threads_num; ++thrd) {
    int RowBeg = thrd * SizeBlockForThread;
    int RowEnd = std::min((thrd + 1) * SizeBlockForThread, n);

    threads[thrd] = std::thread([&, RowBeg, RowEnd] {
      std::vector<double> loc_mtrx_C(n * m, 0.0);

      for (int i = RowBeg; i < RowEnd; i += SizeBlock) {
        for (int j = 0; j < m; j += SizeBlock) {
          for (int k = 0; k < m; k += SizeBlock) {
            BlocksMultiply(A, B, loc_mtrx_C, n, m, SizeBlock, i, j, k);
          }
        }
      }

      for (int i = RowBeg; i < RowEnd; ++i) {
        for (int j = 0; j < m; ++j) {
          mtrx_C[i * m + j] += loc_mtrx_C[i * m + j];
        }
      }
    });
  }

  for (auto& thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  return mtrx_C;
}

std::vector<double> multiplyMtrx(const std::vector<double>& A, const std::vector<double>& B, int rows_A, int col_B) {
  int col_A = rows_A;
  std::vector<double> mtrx_C(rows_A * col_B, 0.0);

  if (rows_A == 0 || col_B == 0) {
    return std::vector<double>();
  }

  for (int i = 0; i < rows_A; ++i) {
    for (int j = 0; j < col_B; ++j) {
      for (int k = 0; k < col_A; ++k) {
        mtrx_C[i * col_B + j] += A[i * col_A + k] * B[k * col_B + j];
      }
    }
  }
  return mtrx_C;
}

std::vector<double> RndMatrix(int rows, int cols) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<double> dis(1.0, 20.0);

  std::vector<double> res_mtrx(rows * cols);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      res_mtrx[i * cols + j] = dis(gen);
    }
  }

  return res_mtrx;
}

bool TestTBBSequentialNedelinCannon::pre_processing() {
  internal_order_test();

  A = std::vector<double>(taskData->inputs_count[0]);
  B = std::vector<double>(taskData->inputs_count[1]);
  n = *reinterpret_cast<int*>(taskData->inputs[2]);
  m = *reinterpret_cast<int*>(taskData->inputs[3]);

  auto* tmp_ptr_A = reinterpret_cast<double*>(taskData->inputs[0]);
  for (size_t i = 0; i < taskData->inputs_count[0]; i++) {
    A[i] = tmp_ptr_A[i];
  }

  auto* tmp_ptr_B = reinterpret_cast<double*>(taskData->inputs[1]);
  for (size_t i = 0; i < taskData->inputs_count[1]; i++) {
    B[i] = tmp_ptr_B[i];
  }
  return true;
}

bool TestTBBSequentialNedelinCannon::validation() {
  internal_order_test();

  return taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0] &&
         taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool TestTBBSequentialNedelinCannon::run() {
  internal_order_test();
  res = cannonMtrxMultiplication(A, B, n, m);
  return true;
}

bool TestTBBSequentialNedelinCannon::post_processing() {
  internal_order_test();
  std::copy(res.begin(), res.end(), reinterpret_cast<double*>(taskData->outputs[0]));
  return true;
}

bool TestTaskSTLParallelNedelinCannon::pre_processing() {
  internal_order_test();

  A = std::vector<double>(taskData->inputs_count[0]);
  B = std::vector<double>(taskData->inputs_count[1]);

  n = *reinterpret_cast<int*>(taskData->inputs[2]);
  m = *reinterpret_cast<int*>(taskData->inputs[3]);

  auto* tmp_ptr_A = reinterpret_cast<double*>(taskData->inputs[0]);
  for (size_t i = 0; i < taskData->inputs_count[0]; i++) {
    A[i] = tmp_ptr_A[i];
  }

  auto* tmp_ptr_B = reinterpret_cast<double*>(taskData->inputs[1]);
  for (size_t i = 0; i < taskData->inputs_count[1]; i++) {
    B[i] = tmp_ptr_B[i];
  }
  return true;
}

bool TestTaskSTLParallelNedelinCannon::validation() {
  internal_order_test();

  return taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0] &&
         taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool TestTaskSTLParallelNedelinCannon::run() {
  internal_order_test();
  res = cannonMtrxMultiplication_tbb(A, B, n, m);
  return true;
}

bool TestTaskSTLParallelNedelinCannon::post_processing() {
  internal_order_test();
  std::copy(res.begin(), res.end(), reinterpret_cast<double*>(taskData->outputs[0]));
  return true;
}