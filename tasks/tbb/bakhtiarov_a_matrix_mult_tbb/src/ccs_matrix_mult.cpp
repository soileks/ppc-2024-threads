// Copyright 2024 Bakhtiarov Alexander

#include "tbb/bakhtiarov_a_matrix_mult_css_tbb/include/ccs_matrix_mult.hpp"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

using namespace std;

void SparseTBBMatrixMulti::construct_ccs(Matrix& matrix, const double* data, int numRows, int numCols) {
  int notNullNumbers = 0;
  for (int j = 0; j < numCols; ++j) {
    matrix.colPtr.push_back(notNullNumbers);
    for (int i = 0; i < numRows; ++i) {
      int index = i * numCols + j;
      if (data[index] != 0) {
        matrix.values.push_back(data[index]);
        matrix.rows.push_back(i);
        notNullNumbers++;
      }
    }
  }
  matrix.colPtr.push_back(notNullNumbers);
}

bool SparseTBBMatrixMulti::pre_processing() {
  internal_order_test();

  auto* matrix1Data = reinterpret_cast<double*>(taskData->inputs[0]);
  auto* matrix2Data = reinterpret_cast<double*>(taskData->inputs[1]);

  matrix1.numRows = taskData->inputs_count[0];
  matrix1.numCols = taskData->inputs_count[1];
  matrix2.numRows = taskData->inputs_count[2];
  matrix2.numCols = taskData->inputs_count[3];

  result.numRows = matrix1.numRows;
  result.numCols = matrix2.numCols;

  result.values.resize(matrix1.numRows * matrix2.numCols, 0);

  construct_ccs(matrix1, matrix1Data, matrix1.numRows, matrix1.numCols);
  construct_ccs(matrix2, matrix2Data, matrix2.numRows, matrix2.numCols);

  return true;
}

bool SparseTBBMatrixMulti::validation() {
  internal_order_test();
  return taskData->inputs_count[1] == taskData->inputs_count[2] &&
         taskData->outputs_count[0] == taskData->inputs_count[0] &&
         taskData->outputs_count[1] == taskData->inputs_count[3];
}

bool SparseTBBMatrixMulti::post_processing() {
  internal_order_test();

  auto* out_ptr = reinterpret_cast<double*>(taskData->outputs[0]);
  copy(result.values.begin(), result.values.end(), out_ptr);

  return true;
}

bool SparseTBBMatrixMultiSequential::run() {
  internal_order_test();

  for (int j = 0; j < matrix1.numCols; j++) {
    for (int k = matrix2.colPtr[j]; k < matrix2.colPtr[j + 1]; k++) {
      int column2 = j;
      int row2 = matrix2.rows[k];
      for (int l = matrix1.colPtr[row2]; l < matrix1.colPtr[row2 + 1]; l++) {
        int row1 = matrix1.rows[l];
        double val1 = matrix1.values[l];
        double val2 = matrix2.values[k];
        result.values[row1 * matrix2.numCols + column2] += val1 * val2;
      }
    }
  }

  return true;
}

bool SparseTBBMatrixMultiParallel::run() {
  internal_order_test();

  tbb::parallel_for(0, matrix1.numCols, [&](int j) {
    for (int k = matrix2.colPtr[j]; k < matrix2.colPtr[j + 1]; k++) {
      int column2 = j;
      int row2 = matrix2.rows[k];
      for (int l = matrix1.colPtr[row2]; l < matrix1.colPtr[row2 + 1]; l++) {
        int row1 = matrix1.rows[l];
        double val1 = matrix1.values[l];
        double val2 = matrix2.values[k];
        result.values
         result.values[row1 * matrix2.numCols + column2] += val1 * val2;
      }
    }
  });

  return true;
}
