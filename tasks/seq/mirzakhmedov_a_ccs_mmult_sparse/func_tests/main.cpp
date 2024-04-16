// Copyright 2024 Mirzakhmedov Alexander
#include <gtest/gtest.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#include "seq/mirzakhmedov_a_ccs_mmult_sparse/include/ccs_mmult_sparse.hpp"

const double PI = 3.14159265358979323846;
/*
sparse_matrix dft_matrix(int n) {
  auto N = (double)n;
  std::complex<double> exponent{0.0, -2.0 * PI / N};
  sparse_matrix dft(n, n, n * n);
  for (int i = 1; i <= n; ++i) {
    dft.col_ptr[i] = i * n;
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      dft.rows[i * n + j] = j;
      dft.values[i * n + j] = std::exp(exponent * double(i * j));
    }
  }
  return dft;
}

sparse_matrix dft_conj_matrix(int n) {
  auto N = (double)n;
  std::complex<double> exponent{0.0, 2.0 * PI / N};
  sparse_matrix dft_conj(n, n, n * n);
  for (int i = 1; i <= n; ++i) {
    dft_conj.col_ptr[i] = i * n;
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      dft_conj.rows[i * n + j] = j;
      dft_conj.values[i * n + j] = std::exp(exponent * double(j * i));
    }
  }
  return dft_conj;
}
*/

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_I) {
  MatrixInCCS_Sparse M1(1, 1, 1);
  MatrixInCCS_Sparse M2(1, 1, 1);
  MatrixInCCS_Sparse M3;

  M1.col_p = {0, 1};
  M1.row = {0};
  M1.val = {std::complex<double>(0.0, 1.0)};

  M2.col_p = {0, 1};
  M2.row = {0};
  M2.val = {std::complex<double>(0.0, -1.0)};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  SpgemmCSCComplexSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  std::complex<double> answer(1.0, 0.0);
  ASSERT_NEAR(std::abs(C.val[0] - answer), 0.0, 1e-6);
}
