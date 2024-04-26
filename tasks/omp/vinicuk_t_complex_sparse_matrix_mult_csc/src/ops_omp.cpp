// Copyright 2024 Vinichuk Timofey

#include "omp/vinicuk_t_complex_sparse_matrix_mult_csc/include/ops_omp.hpp"

#include <omp.h>

#include <algorithm>
#include <complex>
#include <iostream>

bool vinichuk_t_omp::MultMatrixCSCComplex::pre_processing() {
  internal_order_test();

  mtrx_A = reinterpret_cast<CSCComplexMatrix*>(taskData->inputs[0]);
  mtrx_B = reinterpret_cast<CSCComplexMatrix*>(taskData->inputs[1]);
  mtrx_res = reinterpret_cast<CSCComplexMatrix*>(taskData->outputs[0]);
  return true;
}

bool vinichuk_t_omp::MultMatrixCSCComplex::validation() {
  internal_order_test();

  int res_col_num = reinterpret_cast<CSCComplexMatrix*>(taskData->outputs[0])->num_cols;
  int res_row_num = reinterpret_cast<CSCComplexMatrix*>(taskData->outputs[0])->num_rows;
  int A_col_num = reinterpret_cast<CSCComplexMatrix*>(taskData->inputs[0])->num_cols;
  int A_row_num = reinterpret_cast<CSCComplexMatrix*>(taskData->inputs[0])->num_rows;
  int B_col_num = reinterpret_cast<CSCComplexMatrix*>(taskData->inputs[1])->num_cols;
  int B_row_num = reinterpret_cast<CSCComplexMatrix*>(taskData->inputs[1])->num_rows;

  return A_col_num == B_row_num && res_col_num == B_col_num && res_row_num == A_row_num;
}

bool vinichuk_t_omp::MultMatrixCSCComplex::run() {
  internal_order_test();

  std::vector<std::complex<double>> values;
  std::vector<int> row_indexes;
  std::vector<int> col_ptrs;
  std::complex<double> sum;

  std::vector<std::vector<std::complex<double>>> temp_values(mtrx_B->num_cols, std::vector<std::complex<double>>());
  std::vector<std::vector<int>> temp_row_indexes(mtrx_B->num_cols, std::vector<int>());
  mtrx_res->col_ptrs.push_back(0);

#pragma omp parallel for private(sum) schedule(dynamic)
  for (int j = 0; j < mtrx_B->num_cols; j++) {
    // std::vector<std::complex<double>> temp_values;
    // std::vector<int> temp_row_indexes;
    for (int i = 0; i < mtrx_A->num_rows; i++) {
      sum.imag(0.0);
      sum.real(0.0);
      for (int k = mtrx_B->col_ptrs[j]; k < mtrx_B->col_ptrs[j + 1]; k++) {
        auto start = mtrx_A->row_indexes.begin() + mtrx_A->col_ptrs[mtrx_B->row_indexes[k]];
        auto end = mtrx_A->row_indexes.begin() + mtrx_A->col_ptrs[mtrx_B->row_indexes[k] + 1];
        auto it = find(start, end, i);
        if (it != end) {
          int index = distance(mtrx_A->row_indexes.begin(), it);
          sum += mtrx_A->values[index] * mtrx_B->values[k];
        }
      }
      if (sum.imag() != 0.0 || sum.real() != 0.0) {
        temp_values[j].push_back(sum);
        temp_row_indexes[j].push_back(i);
      }
    }
  }
  for (int i = 0; i < temp_values.size(); i++) {
    mtrx_res->col_ptrs.push_back(mtrx_res->values.size() + temp_values[i].size());
    mtrx_res->values.insert(mtrx_res->values.end(), temp_values[i].begin(), temp_values[i].end());
    mtrx_res->row_indexes.insert(mtrx_res->row_indexes.end(), temp_row_indexes[i].begin(), temp_row_indexes[i].end());
  }
  return true;
}

bool vinichuk_t_omp::MultMatrixCSCComplex::post_processing() {
  internal_order_test();
  return true;
}
