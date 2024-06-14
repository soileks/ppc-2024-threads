// Copyright 2024 Vinichuk Timofey
#pragma once

#include <complex>
#include <random>
#include <vector>

struct CSCComplexMatrix {
  std::vector<std::complex<double>> values;
  std::vector<int> row_indexes;
  std::vector<int> col_ptrs;

  int num_rows;
  int num_cols;

  // CSSComplexMatrix(size_t n_rows, size_t n_cols);

  CSCComplexMatrix(int _num_cols, int _num_rows) : num_rows(_num_rows), num_cols(_num_cols) {}
};
