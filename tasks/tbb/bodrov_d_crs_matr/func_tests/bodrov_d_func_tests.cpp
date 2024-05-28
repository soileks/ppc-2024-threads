// Copyright 2024 Bodrov Daniil
#include <gtest/gtest.h>

#include <complex>
#include <vector>

#include "tbb/bodrov_d_crs_matr/include/bodrov_d_crs_matr_tbb.hpp"
using namespace bodrov_tbb;

TEST(bodrov_d_crs_matr_omp, test_identity_matrix) {
  SparseMatrixBodrovOMP A;
  A.n_rows = 3;
  A.n_cols = 3;
  A.pointer = {0, 1, 2, 3};
  A.col_indexes = {0, 1, 2};
  A.non_zero_values = {1, 1, 1};

  SparseMatrixBodrovOMP B;
  B.n_rows = 3;
  B.n_cols = 3;
  B.pointer = {0, 1, 2, 3};
  B.col_indexes = {0, 1, 2};
  B.non_zero_values = {1, 1, 1};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.n_rows = 3;
  Expected.n_cols = 3;
  Expected.pointer = {0, 1, 2, 3};
  Expected.col_indexes = {0, 1, 2};
  Expected.non_zero_values = {1, 1, 1};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Seq.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Seq.pointer, Expected.pointer);
  ASSERT_EQ(Result_Seq.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Seq.non_zero_values.size(), Expected.non_zero_values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Par.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Par.pointer, Expected.pointer);
  ASSERT_EQ(Result_Par.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Par.non_zero_values.size(), Expected.non_zero_values.size());

  for (size_t i = 0; i < Result_Par.non_zero_values.size(); i++) {
    std::complex<double> t = Result_Par.non_zero_values[i] - Result_Seq.non_zero_values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_zero_matrix) {
  SparseMatrixBodrovOMP A;
  A.n_rows = 3;
  A.n_cols = 3;
  A.pointer = {0, 0, 0, 0};
  A.col_indexes = {};
  A.non_zero_values = {};

  SparseMatrixBodrovOMP B;
  B.n_rows = 3;
  B.n_cols = 3;
  B.pointer = {0, 0, 0, 0};
  B.col_indexes = {};
  B.non_zero_values = {};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.n_rows = 3;
  Expected.n_cols = 3;
  Expected.pointer = {0, 0, 0, 0};
  Expected.col_indexes = {};
  Expected.non_zero_values = {};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Seq.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Seq.pointer, Expected.pointer);
  ASSERT_EQ(Result_Seq.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Seq.non_zero_values.size(), Expected.non_zero_values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Par.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Par.pointer, Expected.pointer);
  ASSERT_EQ(Result_Par.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Par.non_zero_values.size(), Expected.non_zero_values.size());

  for (size_t i = 0; i < Result_Par.non_zero_values.size(); i++) {
    std::complex<double> t = Result_Par.non_zero_values[i] - Result_Seq.non_zero_values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_large_matrix) {
  SparseMatrixBodrovOMP A;
  A.n_rows = 1000;
  A.n_cols = 1000;
  for (int i = 0; i <= 1000; ++i) {
    A.pointer.push_back(i);
  }
  for (int i = 0; i < 1000; ++i) {
    A.col_indexes.push_back(i);
    A.non_zero_values.push_back(1.0);
  }

  SparseMatrixBodrovOMP B;
  B.n_rows = 1000;
  B.n_cols = 1000;
  for (int i = 0; i <= 1000; ++i) {
    B.pointer.push_back(i);
  }
  for (int i = 0; i < 1000; ++i) {
    B.col_indexes.push_back(i);
    B.non_zero_values.push_back(1.0);
  }

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.n_rows = 1000;
  Expected.n_cols = 1000;
  for (int i = 0; i <= 1000; ++i) {
    Expected.pointer.push_back(i);
  }
  for (int i = 0; i < 1000; ++i) {
    Expected.col_indexes.push_back(i);
    Expected.non_zero_values.push_back(1.0);
  }

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Seq.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Seq.pointer, Expected.pointer);
  ASSERT_EQ(Result_Seq.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Seq.non_zero_values.size(), Expected.non_zero_values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Par.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Par.pointer, Expected.pointer);
  ASSERT_EQ(Result_Par.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Par.non_zero_values.size(), Expected.non_zero_values.size());

  for (size_t i = 0; i < Result_Par.non_zero_values.size(); i++) {
    std::complex<double> t = Result_Par.non_zero_values[i] - Result_Seq.non_zero_values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_non_square_matrix) {
  SparseMatrixBodrovOMP A;
  A.n_rows = 3;
  A.n_cols = 2;
  A.pointer = {0, 1, 2, 3};
  A.col_indexes = {0, 1, 0};
  A.non_zero_values = {1, 1, 1};

  SparseMatrixBodrovOMP B;
  B.n_rows = 2;
  B.n_cols = 4;
  B.pointer = {0, 2, 4};
  B.col_indexes = {0, 1, 2, 3};
  B.non_zero_values = {1, 2, 3, 4};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.n_rows = 3;
  Expected.n_cols = 4;
  Expected.pointer = {0, 2, 4, 6};
  Expected.col_indexes = {0, 1, 2, 3, 0, 1};
  Expected.non_zero_values = {1, 2, 1, 2, 1, 2};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Seq.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Seq.pointer, Expected.pointer);
  ASSERT_EQ(Result_Seq.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Seq.non_zero_values.size(), Expected.non_zero_values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Par.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Par.pointer, Expected.pointer);
  ASSERT_EQ(Result_Par.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Par.non_zero_values.size(), Expected.non_zero_values.size());

  for (size_t i = 0; i < Result_Par.non_zero_values.size(); i++) {
    std::complex<double> t = Result_Par.non_zero_values[i] - Result_Seq.non_zero_values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_mixed_values_matrix) {
  SparseMatrixBodrovOMP A;
  A.n_rows = 3;
  A.n_cols = 3;
  A.pointer = {0, 2, 4, 5};
  A.col_indexes = {0, 2, 1, 2, 0};
  A.non_zero_values = {1.0, 3.0, 2.0, 1.0, 4.0};

  SparseMatrixBodrovOMP B;
  B.n_rows = 3;
  B.n_cols = 3;
  B.pointer = {0, 1, 3, 4};
  B.col_indexes = {0, 1, 2, 2};
  B.non_zero_values = {1.0, 2.0, 3.0, 4.0};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.n_rows = 3;
  Expected.n_cols = 3;
  Expected.pointer = {0, 2, 4, 5};
  Expected.col_indexes = {0, 2, 1, 2, 0};
  Expected.non_zero_values = {1.0, 12.0, 4.0, 3.0, 16.0};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Seq.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Seq.pointer, Expected.pointer);
  ASSERT_EQ(Result_Seq.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Seq.non_zero_values.size(), Expected.non_zero_values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.n_rows, Expected.n_rows);
  ASSERT_EQ(Result_Par.n_cols, Expected.n_cols);
  ASSERT_EQ(Result_Par.pointer, Expected.pointer);
  ASSERT_EQ(Result_Par.col_indexes, Expected.col_indexes);
  ASSERT_EQ(Result_Par.non_zero_values.size(), Expected.non_zero_values.size());

  for (size_t i = 0; i < Result_Par.non_zero_values.size(); i++) {
    std::complex<double> t = Result_Par.non_zero_values[i] - Result_Seq.non_zero_values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}
