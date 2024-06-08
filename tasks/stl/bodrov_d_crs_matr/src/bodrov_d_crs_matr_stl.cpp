// Copyright 2024 Bodrov Daniil
#include "stl/bodrov_d_crs_matr/include/bodrov_d_crs_matr_stl.hpp"

#include <algorithm>
#include <complex>
#include <future>
#include <mutex>
#include <numeric>
#include <thread>
#include <utility>
#include <vector>
#include <functional>

using namespace std::chrono_literals;
using namespace bodrov_stl;

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

  Result->Rows = MatrixA->Rows;
  Result->Columns = MatrixB->Columns;
  Result->DataPointer.assign(MatrixA->Rows + 1, 0);

  std::vector<std::mutex> row_mutexes(MatrixA->Rows);
  std::vector<std::vector<int>> temp_col_indexes(MatrixA->Rows);
  std::vector<std::vector<std::complex<double>>> temp_values(MatrixA->Rows);

  auto multiply_row = [&](int row) {
    for (int ja = MatrixA->DataPointer[row]; ja < MatrixA->DataPointer[row + 1]; ++ja) {
      int col_a = MatrixA->ColumnsIndexes[ja];
      std::complex<double> value_a = MatrixA->Values[ja];

      for (int jb = MatrixB->DataPointer[col_a]; jb < MatrixB->DataPointer[col_a + 1]; ++jb) {
        int col_b = MatrixB->ColumnsIndexes[jb];
        std::complex<double> value_b = MatrixB->Values[jb];

        std::lock_guard<std::mutex> lock(row_mutexes[row]);
        auto it = std::find(temp_col_indexes[row].begin(), temp_col_indexes[row].end(), col_b);
        if (it != temp_col_indexes[row].end()) {
          int index = std::distance(temp_col_indexes[row].begin(), it);
          temp_values[row][index] += value_a * value_b;
        } else {
          temp_col_indexes[row].push_back(col_b);
          temp_values[row].push_back(value_a * value_b);
        }
      }
    }
  };

  const auto nthreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(nthreads);

  for (unsigned t = 0; t < nthreads; ++t) {
    threads[t] = std::thread([&, t]() {
      for (int row = t; row < MatrixA->Rows; row += nthreads) {
        multiply_row(row);
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  for (int i = 0; i < MatrixA->Rows; ++i) {
    Result->DataPointer[i + 1] = temp_col_indexes[i].size();
    Result->ColumnsIndexes.insert(Result->ColumnsIndexes.end(), temp_col_indexes[i].begin(), temp_col_indexes[i].end());
    Result->Values.insert(Result->Values.end(), temp_values[i].begin(), temp_values[i].end());
  }

  std::partial_sum(Result->DataPointer.begin(), Result->DataPointer.end(), Result->DataPointer.begin());

  return true;
}

bool SparseMatrixSolverBodrovOMPParallel::post_processing() {
  internal_order_test();
  return true;
}
