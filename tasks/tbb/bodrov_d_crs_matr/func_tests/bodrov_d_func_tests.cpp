// Copyright 2024 Bodrov Daniil
#include <gtest/gtest.h>

#include <complex>
#include <vector>

#include "tbb/bodrov_d_crs_matr/include/bodrov_d_crs_matr_tbb.hpp"
using namespace bodrov_tbb;

TEST(bodrov_d_crs_matr_omp, test_identity_matrix) {
  SparseMatrixBodrovOMP A;
  A.Rows = 3;
  A.Columns = 3;
  A.DataPointer = {0, 1, 2, 3};
  A.ColumnsIndexes = {0, 1, 2};
  A.Values = {1, 1, 1};

  SparseMatrixBodrovOMP B;
  B.Rows = 3;
  B.Columns = 3;
  B.DataPointer = {0, 1, 2, 3};
  B.ColumnsIndexes = {0, 1, 2};
  B.Values = {1, 1, 1};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.Rows = 3;
  Expected.Columns = 3;
  Expected.DataPointer = {0, 1, 2, 3};
  Expected.ColumnsIndexes = {0, 1, 2};
  Expected.Values = {1, 1, 1};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.Rows, Expected.Rows);
  ASSERT_EQ(Result_Seq.Columns, Expected.Columns);
  ASSERT_EQ(Result_Seq.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Seq.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Seq.Values.size(), Expected.Values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.Rows, Expected.Rows);
  ASSERT_EQ(Result_Par.Columns, Expected.Columns);
  ASSERT_EQ(Result_Par.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Par.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Par.Values.size(), Expected.Values.size());

  for (size_t i = 0; i < Result_Par.Values.size(); i++) {
    std::complex<double> t = Result_Par.Values[i] - Result_Seq.Values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_zero_matrix) {
  SparseMatrixBodrovOMP A;
  A.Rows = 3;
  A.Columns = 3;
  A.DataPointer = {0, 0, 0, 0};
  A.ColumnsIndexes = {};
  A.Values = {};

  SparseMatrixBodrovOMP B;
  B.Rows = 3;
  B.Columns = 3;
  B.DataPointer = {0, 0, 0, 0};
  B.ColumnsIndexes = {};
  B.Values = {};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.Rows = 3;
  Expected.Columns = 3;
  Expected.DataPointer = {0, 0, 0, 0};
  Expected.ColumnsIndexes = {};
  Expected.Values = {};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.Rows, Expected.Rows);
  ASSERT_EQ(Result_Seq.Columns, Expected.Columns);
  ASSERT_EQ(Result_Seq.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Seq.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Seq.Values.size(), Expected.Values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.Rows, Expected.Rows);
  ASSERT_EQ(Result_Par.Columns, Expected.Columns);
  ASSERT_EQ(Result_Par.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Par.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Par.Values.size(), Expected.Values.size());

  for (size_t i = 0; i < Result_Par.Values.size(); i++) {
    std::complex<double> t = Result_Par.Values[i] - Result_Seq.Values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_large_matrix) {
  SparseMatrixBodrovOMP A;
  A.Rows = 1000;
  A.Columns = 1000;
  for (int i = 0; i <= 1000; ++i) {
    A.DataPointer.emplace_back(i);
  }
  for (int i = 0; i < 1000; ++i) {
    A.ColumnsIndexes.emplace_back(i);
    A.Values.emplace_back(1.0);
  }

  SparseMatrixBodrovOMP B;
  B.Rows = 1000;
  B.Columns = 1000;
  for (int i = 0; i <= 1000; ++i) {
    B.DataPointer.emplace_back(i);
  }
  for (int i = 0; i < 1000; ++i) {
    B.ColumnsIndexes.emplace_back(i);
    B.Values.emplace_back(1.0);
  }

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.Rows = 1000;
  Expected.Columns = 1000;
  for (int i = 0; i <= 1000; ++i) {
    Expected.DataPointer.emplace_back(i);
  }
  for (int i = 0; i < 1000; ++i) {
    Expected.ColumnsIndexes.emplace_back(i);
    Expected.Values.emplace_back(1.0);
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

  ASSERT_EQ(Result_Seq.Rows, Expected.Rows);
  ASSERT_EQ(Result_Seq.Columns, Expected.Columns);
  ASSERT_EQ(Result_Seq.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Seq.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Seq.Values.size(), Expected.Values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.Rows, Expected.Rows);
  ASSERT_EQ(Result_Par.Columns, Expected.Columns);
  ASSERT_EQ(Result_Par.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Par.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Par.Values.size(), Expected.Values.size());

  for (size_t i = 0; i < Result_Par.Values.size(); i++) {
    std::complex<double> t = Result_Par.Values[i] - Result_Seq.Values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_non_square_matrix) {
  SparseMatrixBodrovOMP A;
  A.Rows = 3;
  A.Columns = 2;
  A.DataPointer = {0, 1, 2, 3};
  A.ColumnsIndexes = {0, 1, 0};
  A.Values = {1, 1, 1};

  SparseMatrixBodrovOMP B;
  B.Rows = 2;
  B.Columns = 4;
  B.DataPointer = {0, 2, 4};
  B.ColumnsIndexes = {0, 1, 2, 3};
  B.Values = {1, 2, 3, 4};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.Rows = 3;
  Expected.Columns = 4;
  Expected.DataPointer = {0, 2, 4, 6};
  Expected.ColumnsIndexes = {0, 1, 2, 3, 0, 1};
  Expected.Values = {1, 2, 1, 2, 1, 2};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.Rows, Expected.Rows);
  ASSERT_EQ(Result_Seq.Columns, Expected.Columns);
  ASSERT_EQ(Result_Seq.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Seq.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Seq.Values.size(), Expected.Values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.Rows, Expected.Rows);
  ASSERT_EQ(Result_Par.Columns, Expected.Columns);
  ASSERT_EQ(Result_Par.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Par.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Par.Values.size(), Expected.Values.size());

  for (size_t i = 0; i < Result_Par.Values.size(); i++) {
    std::complex<double> t = Result_Par.Values[i] - Result_Seq.Values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}

TEST(bodrov_d_crs_matr_omp, test_mixed_values_matrix) {
  SparseMatrixBodrovOMP A;
  A.Rows = 3;
  A.Columns = 3;
  A.DataPointer = {0, 2, 4, 5};
  A.ColumnsIndexes = {0, 2, 1, 2, 0};
  A.Values = {1.0, 3.0, 2.0, 1.0, 4.0};

  SparseMatrixBodrovOMP B;
  B.Rows = 3;
  B.Columns = 3;
  B.DataPointer = {0, 1, 3, 4};
  B.ColumnsIndexes = {0, 1, 2, 2};
  B.Values = {1.0, 2.0, 3.0, 4.0};

  SparseMatrixBodrovOMP Result_Seq;
  SparseMatrixBodrovOMP Result_Par;

  SparseMatrixBodrovOMP Expected;
  Expected.Rows = 3;
  Expected.Columns = 3;
  Expected.DataPointer = {0, 2, 4, 5};
  Expected.ColumnsIndexes = {0, 2, 1, 2, 0};
  Expected.Values = {1.0, 12.0, 4.0, 3.0, 16.0};

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Seq));

  SparseMatrixSolverBodrovOMP taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  ASSERT_EQ(Result_Seq.Rows, Expected.Rows);
  ASSERT_EQ(Result_Seq.Columns, Expected.Columns);
  ASSERT_EQ(Result_Seq.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Seq.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Seq.Values.size(), Expected.Values.size());

  auto taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result_Par));

  SparseMatrixSolverBodrovOMPParallel taskParallel(taskDataPar);
  ASSERT_EQ(taskParallel.validation(), true);
  ASSERT_EQ(taskParallel.pre_processing(), true);
  ASSERT_EQ(taskParallel.run(), true);
  ASSERT_EQ(taskParallel.post_processing(), true);

  ASSERT_EQ(Result_Par.Rows, Expected.Rows);
  ASSERT_EQ(Result_Par.Columns, Expected.Columns);
  ASSERT_EQ(Result_Par.DataPointer, Expected.DataPointer);
  ASSERT_EQ(Result_Par.ColumnsIndexes, Expected.ColumnsIndexes);
  ASSERT_EQ(Result_Par.Values.size(), Expected.Values.size());

  for (size_t i = 0; i < Result_Par.Values.size(); i++) {
    std::complex<double> t = Result_Par.Values[i] - Result_Seq.Values[i];
    ASSERT_NEAR(0.0f, t.imag(), 1e-3);
    ASSERT_NEAR(0.0f, t.real(), 1e-3);
  }
}
