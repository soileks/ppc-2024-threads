// Copyright 2024 Bodrov Daniil
#include "tbb/bodrov_d_crs_matr/include/bodrov_d_crs_matr_tbb.hpp"

#include <oneapi/tbb.h>

#include <algorithm>
#include <complex>
#include <utility>
#include <vector>

using namespace std::chrono_literals;
using namespace bodrov_tbb;

bool SparseMatrixSolverBodrovOMP::pre_processing() {
  internal_order_test();
  A_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  B_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  // Инициализация Result
  Result->n_rows = A_M->n_rows;
  Result->n_cols = B_M->n_cols;
  Result->pointer.assign(A_M->n_rows + 1, 0);

  return true;
}

bool SparseMatrixSolverBodrovOMP::validation() {
  internal_order_test();

  if (taskData->inputs[0] != nullptr || taskData->inputs[1] != nullptr || taskData->outputs[0] != nullptr) {
    return false;
  }

  A_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  B_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  if (A_M->n_cols != B_M->n_rows) {
    return false;
  }

  if (A_M->n_rows <= 0 || A_M->n_cols <= 0 || B_M->n_rows <= 0 || B_M->n_cols <= 0) {
    return false;
  }

  if (A_M->pointer.size() != static_cast<size_t>(A_M->n_rows + 1)) {
    return false;
  }
  if (B_M->pointer.size() != static_cast<size_t>(B_M->n_rows + 1)) {
    return false;
  }

  if (A_M->non_zero_values.size() != A_M->col_indexes.size()) {
    return false;
  }
  if (B_M->non_zero_values.size() != B_M->col_indexes.size()) {
    return false;
  }

  return true;
}

bool SparseMatrixSolverBodrovOMP::run() {
  internal_order_test();

  std::vector<std::vector<std::complex<double>>> temp_result(
      A_M->n_rows, std::vector<std::complex<double>>(B_M->n_cols, {0.0, 0.0}));

  for (int i = 0; i < A_M->n_rows; ++i) {
    for (int j = A_M->pointer[i]; j < A_M->pointer[i + 1]; ++j) {
      int col_A = A_M->col_indexes[j];
      std::complex<double> val_A = A_M->non_zero_values[j];

      for (int k = B_M->pointer[col_A]; k < B_M->pointer[col_A + 1]; ++k) {
        int col_B = B_M->col_indexes[k];
        std::complex<double> val_B = B_M->non_zero_values[k];
        temp_result[i][col_B] += val_A * val_B;
      }
    }
  }

  Result->non_zero_values.clear();
  Result->col_indexes.clear();

  for (int i = 0; i < A_M->n_rows; ++i) {
    for (int j = 0; j < B_M->n_cols; ++j) {
      if (temp_result[i][j] != std::complex<double>(0.0, 0.0)) {
        Result->non_zero_values.emplace_back(temp_result[i][j]);
        Result->col_indexes.emplace_back(j);
      }
    }
    Result->pointer[i + 1] = Result->non_zero_values.size();
  }

  return true;
}

bool SparseMatrixSolverBodrovOMP::post_processing() {
  internal_order_test();
  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::pre_processing() {
  internal_order_test();
  A_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  B_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  Result->n_rows = A_M->n_rows;
  Result->n_cols = B_M->n_cols;
  Result->pointer.assign(A_M->n_rows + 1, 0);

  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::validation() {
  internal_order_test();

  if (taskData->inputs[0] != nullptr || taskData->inputs[1] != nullptr || taskData->outputs[0] != nullptr) {
    return false;
  }

  A_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  B_M = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  if (A_M->n_cols != B_M->n_rows) {
    return false;
  }

  if (A_M->n_rows <= 0 || A_M->n_cols <= 0 || B_M->n_rows <= 0 || B_M->n_cols <= 0) {
    return false;
  }

  if (A_M->pointer.size() != static_cast<size_t>(A_M->n_rows + 1)) {
    return false;
  }
  if (B_M->pointer.size() != static_cast<size_t>(B_M->n_rows + 1)) {
    return false;
  }

  if (A_M->non_zero_values.size() != A_M->col_indexes.size()) {
    return false;
  }
  if (B_M->non_zero_values.size() != B_M->col_indexes.size()) {
    return false;
  }

  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::run() {
  internal_order_test();

  std::vector<std::vector<std::complex<double>>> temp_result(
      A_M->n_rows, std::vector<std::complex<double>>(B_M->n_cols, {0.0, 0.0}));

  tbb::parallel_for(0, A_M->n_rows, [&](int i) {
    for (int j = A_M->pointer[i]; j < A_M->pointer[i + 1]; ++j) {
      int col_A = A_M->col_indexes[j];
      std::complex<double> val_A = A_M->non_zero_values[j];

      for (int k = B_M->pointer[col_A]; k < B_M->pointer[col_A + 1]; ++k) {
        int col_B = B_M->col_indexes[k];
        std::complex<double> val_B = B_M->non_zero_values[k];
        temp_result[i][col_B] += val_A * val_B;
      }
    }
  });

  Result->non_zero_values.clear();
  Result->col_indexes.clear();

  for (int i = 0; i < A_M->n_rows; ++i) {
    for (int j = 0; j < B_M->n_cols; ++j) {
      if (temp_result[i][j] != std::complex<double>(0.0, 0.0)) {
        Result->non_zero_values.emplace_back(temp_result[i][j]);
        Result->col_indexes.emplace_back(j);
      }
    }
    Result->pointer[i + 1] = Result->non_zero_values.size();
  }

  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::post_processing() {
  internal_order_test();
  return true;
}
