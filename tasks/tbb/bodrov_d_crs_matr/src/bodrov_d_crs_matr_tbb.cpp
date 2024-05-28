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
  MatrixA = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  MatrixB = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  // Инициализация Result
  Result->Rows = MatrixA->Rows;
  Result->Columns = MatrixB->Columns;
  Result->DataPointer.assign(MatrixA->Rows + 1, 0);

  return true;
}

bool SparseMatrixSolverBodrovOMP::validation() {
  internal_order_test();

  if (taskData->inputs.size() != 2 || taskData->outputs.size() != 1 || !taskData->inputs_count.empty() ||
      !taskData->outputs_count.empty())
    return false;

  MatrixA = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  MatrixB = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  if (MatrixA->Columns != MatrixB->Rows) {
    return false;
  }

  if (MatrixA->Rows <= 0 || MatrixA->Columns <= 0 || MatrixB->Rows <= 0 || MatrixB->Columns <= 0) {
    return false;
  }

  if (MatrixA->DataPointer.size() != static_cast<size_t>(MatrixA->Rows + 1)) {
    return false;
  }
  if (MatrixB->DataPointer.size() != static_cast<size_t>(MatrixB->Rows + 1)) {
    return false;
  }

  if (MatrixA->Values.size() != MatrixA->ColumnsIndexes.size()) {
    return false;
  }
  if (MatrixB->Values.size() != MatrixB->ColumnsIndexes.size()) {
    return false;
  }

  return true;
}

bool SparseMatrixSolverBodrovOMP::run() {
  internal_order_test();

  std::vector<std::vector<std::complex<double>>> temp_result(
      MatrixA->Rows, std::vector<std::complex<double>>(MatrixB->Columns, {0.0, 0.0}));

  for (int i = 0; i < MatrixA->Rows; ++i) {
    for (int j = MatrixA->DataPointer[i]; j < MatrixA->DataPointer[i + 1]; ++j) {
      int col_A = MatrixA->ColumnsIndexes[j];
      std::complex<double> val_A = MatrixA->Values[j];

      for (int k = MatrixB->DataPointer[col_A]; k < MatrixB->DataPointer[col_A + 1]; ++k) {
        int col_B = MatrixB->ColumnsIndexes[k];
        std::complex<double> val_B = MatrixB->Values[k];
        temp_result[i][col_B] += val_A * val_B;
      }
    }
  }

  Result->Values.clear();
  Result->ColumnsIndexes.clear();

  for (int i = 0; i < MatrixA->Rows; ++i) {
    for (int j = 0; j < MatrixB->Columns; ++j) {
      if (temp_result[i][j] != std::complex<double>(0.0, 0.0)) {
        Result->Values.emplace_back(temp_result[i][j]);
        Result->ColumnsIndexes.emplace_back(j);
      }
    }
    Result->DataPointer[i + 1] = Result->Values.size();
  }

  return true;
}

bool SparseMatrixSolverBodrovOMP::post_processing() {
  internal_order_test();
  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::pre_processing() {
  internal_order_test();
  MatrixA = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  MatrixB = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  Result->Rows = MatrixA->Rows;
  Result->Columns = MatrixB->Columns;
  Result->DataPointer.assign(MatrixA->Rows + 1, 0);

  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::validation() {
  internal_order_test();

  if (taskData->inputs.size() != 2 || taskData->outputs.size() != 1 || !taskData->inputs_count.empty() ||
      !taskData->outputs_count.empty())
    return false;

  MatrixA = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[0]);
  MatrixB = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->inputs[1]);
  Result = reinterpret_cast<SparseMatrixBodrovOMP*>(taskData->outputs[0]);

  if (MatrixA->Columns != MatrixB->Rows) {
    return false;
  }

  if (MatrixA->Rows <= 0 || MatrixA->Columns <= 0 || MatrixB->Rows <= 0 || MatrixB->Columns <= 0) {
    return false;
  }

  if (MatrixA->DataPointer.size() != static_cast<size_t>(MatrixA->Rows + 1)) {
    return false;
  }
  if (MatrixB->DataPointer.size() != static_cast<size_t>(MatrixB->Rows + 1)) {
    return false;
  }

  if (MatrixA->Values.size() != MatrixA->ColumnsIndexes.size()) {
    return false;
  }
  if (MatrixB->Values.size() != MatrixB->ColumnsIndexes.size()) {
    return false;
  }

  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::run() {
  internal_order_test();

  std::vector<std::vector<std::complex<double>>> temp_result(
      MatrixA->Rows, std::vector<std::complex<double>>(MatrixB->Columns, {0.0, 0.0}));

  tbb::parallel_for(0, MatrixA->Rows, [&](int i) {
    for (int j = MatrixA->DataPointer[i]; j < MatrixA->DataPointer[i + 1]; ++j) {
      int col_A = MatrixA->ColumnsIndexes[j];
      std::complex<double> val_A = MatrixA->Values[j];

      for (int k = MatrixB->DataPointer[col_A]; k < MatrixB->DataPointer[col_A + 1]; ++k) {
        int col_B = MatrixB->ColumnsIndexes[k];
        std::complex<double> val_B = MatrixB->Values[k];
        temp_result[i][col_B] += val_A * val_B;
      }
    }
  });

  Result->Values.clear();
  Result->ColumnsIndexes.clear();

  for (int i = 0; i < MatrixA->Rows; ++i) {
    for (int j = 0; j < MatrixB->Columns; ++j) {
      if (temp_result[i][j] != std::complex<double>(0.0, 0.0)) {
        Result->Values.emplace_back(temp_result[i][j]);
        Result->ColumnsIndexes.emplace_back(j);
      }
    }
    Result->DataPointer[i + 1] = Result->Values.size();
  }

  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::post_processing() {
  internal_order_test();
  return true;
}
