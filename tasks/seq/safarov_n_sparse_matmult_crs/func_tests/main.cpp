// Copyright 2024 Safarov Nurlan
#include <gtest/gtest.h>

#include <numeric>
#include <vector>

#include "seq/safarov_n_sparse_matmult_crs/include/sparse_matmult_crs.hpp"

TEST(Safarov_N_SparseMatMultCRS, TestOne) {
  // Create data
  SparseMatrixCRS x;
  x.numberOfRows = 3;
  x.numberOfColumns = 2;
  x.pointers = {0, 1, 2, 3};
  x.columnIndexes = {0, 1, 0};
  x.values = {2.5, 0.4, -4.7};

  SparseMatrixCRS y;
  y.numberOfRows = 2;
  y.numberOfColumns = 3;
  y.pointers = {0, 1, 2};
  y.columnIndexes = {1, 2};
  y.values = {-1.2, 7.25};
  SparseMatrixCRS z;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&x));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&y));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&z));

  // Create Task
  SparseMatrixMultiplicationCRS taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  SparseMatrixCRS correctAnswer;
  correctAnswer.numberOfRows = 3;
  correctAnswer.numberOfColumns = 3;
  correctAnswer.pointers = {0, 1, 2, 3};
  correctAnswer.columnIndexes = {1, 2, 1};
  correctAnswer.values = {-3.0, 2.9, 5.64};

  ASSERT_EQ(z.numberOfRows, correctAnswer.numberOfRows);
  ASSERT_EQ(z.numberOfColumns, correctAnswer.numberOfColumns);
  ASSERT_EQ(z.pointers, correctAnswer.pointers);
  ASSERT_EQ(z.columnIndexes, correctAnswer.columnIndexes);
  ASSERT_EQ(z.values.size(), correctAnswer.values.size());
  // ASSERT_EQ(z.values, correctAnswer.values);
  for (size_t i = 0; i < correctAnswer.values.size(); ++i) {
    double t = correctAnswer.values[i] - z.values[i];
    ASSERT_NEAR(0.0f, t, 1e-6);
  }
}

TEST(Safarov_N_SparseMatMultCRS, TestTwo) {
  // Create data
  SparseMatrixCRS x;
  x.numberOfRows = 5;
  x.numberOfColumns = 5;
  x.pointers = {0, 2, 3, 3, 4, 5};
  x.columnIndexes = {0, 2, 1, 2, 1};
  x.values = {2.5, -1.7, 3.4, 0.8, 0.6};

  SparseMatrixCRS identityMatrix;
  identityMatrix.numberOfRows = 5;
  identityMatrix.numberOfColumns = 5;
  identityMatrix.pointers = {0, 1, 2, 3, 4, 5};
  identityMatrix.columnIndexes = {0, 1, 2, 3, 4};
  identityMatrix.values = {1.0, 1.0, 1.0, 1.0, 1.0};

  SparseMatrixCRS oldX;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&x));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&identityMatrix));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&oldX));

  // Create Task
  SparseMatrixMultiplicationCRS taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  SparseMatrixCRS correctAnswer;
  correctAnswer.numberOfRows = 5;
  correctAnswer.numberOfColumns = 5;
  correctAnswer.pointers = {0, 2, 3, 3, 4, 5};
  correctAnswer.columnIndexes = {0, 2, 1, 2, 1};
  correctAnswer.values = {2.5, -1.7, 3.4, 0.8, 0.6};

  ASSERT_EQ(oldX.numberOfRows, correctAnswer.numberOfRows);
  ASSERT_EQ(oldX.numberOfColumns, correctAnswer.numberOfColumns);
  ASSERT_EQ(oldX.pointers, correctAnswer.pointers);
  ASSERT_EQ(oldX.columnIndexes, correctAnswer.columnIndexes);
  ASSERT_EQ(oldX.values.size(), correctAnswer.values.size());
  // ASSERT_EQ(oldX.values, correctAnswer.values);
  for (size_t i = 0; i < correctAnswer.values.size(); ++i) {
    double t = correctAnswer.values[i] - oldX.values[i];
    ASSERT_NEAR(0.0f, t, 1e-6);
  }
}

TEST(Safarov_N_SparseMatMultCRS, TestThree) {
  // Create data
  SparseMatrixCRS x;
  x.numberOfRows = 4;
  x.numberOfColumns = 5;
  x.pointers = {0, 2, 4, 5, 7};
  x.columnIndexes = {0, 3, 2, 4, 1, 0, 3};
  x.values = {3.9, 1.2, 10.45, 5.1, -2.3, 8.6, -0.7};

  SparseMatrixCRS y;
  y.numberOfRows = 5;
  y.numberOfColumns = 4;
  y.pointers = {0, 1, 2, 3, 5, 6};
  y.columnIndexes = {2, 1, 2, 0, 3, 1};
  y.values = {-1.1, 0.5, 3.1, 0.8, -2.7, 1.4};
  SparseMatrixCRS z;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&x));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&y));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&z));

  // Create Task
  SparseMatrixMultiplicationCRS taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  SparseMatrixCRS correctAnswer;
  correctAnswer.numberOfRows = 4;
  correctAnswer.numberOfColumns = 4;
  correctAnswer.pointers = {0, 3, 5, 6, 9};
  correctAnswer.columnIndexes = {0, 2, 3, 1, 2, 1, 0, 2, 3};
  correctAnswer.values = {0.96, -4.29, -3.24, 7.14, 32.395, -1.15, -0.56, -9.46, 1.89};

  ASSERT_EQ(z.numberOfRows, correctAnswer.numberOfRows);
  ASSERT_EQ(z.numberOfColumns, correctAnswer.numberOfColumns);
  ASSERT_EQ(z.pointers, correctAnswer.pointers);
  ASSERT_EQ(z.columnIndexes, correctAnswer.columnIndexes);
  ASSERT_EQ(z.values.size(), correctAnswer.values.size());
  // ASSERT_EQ(z.values, correctAnswer.values);
  for (size_t i = 0; i < correctAnswer.values.size(); ++i) {
    double t = correctAnswer.values[i] - z.values[i];
    ASSERT_NEAR(0.0f, t, 1e-6);
  }
}

TEST(Safarov_N_SparseMatMultCRS, TestFour) {
  // Create data
  SparseMatrixCRS x;
  x.numberOfRows = 6;
  x.numberOfColumns = 6;
  x.pointers = {0, 2, 4, 4, 6, 7, 8};
  x.columnIndexes = {1, 3, 0, 4, 0, 2, 3, 5};
  x.values = {8.0, 2.4, 7.7, 1.3, 1.5, 4.1, 2.5, 6.4};

  SparseMatrixCRS y;
  y.numberOfRows = 6;
  y.numberOfColumns = 6;
  y.pointers = {0, 1, 2, 4, 6, 8, 9};
  y.columnIndexes = {3, 1, 0, 4, 1, 4, 0, 5, 0};
  y.values = {8.5, 4.7, 6.0, 5.1, 2.3, 1.8, 1.1, 0.5, 3.2};
  SparseMatrixCRS z;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&x));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&y));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&z));

  // Create Task
  SparseMatrixMultiplicationCRS taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  SparseMatrixCRS correctAnswer;
  correctAnswer.numberOfRows = 6;
  correctAnswer.numberOfColumns = 6;
  correctAnswer.pointers = {0, 2, 5, 5, 8, 10, 11};
  correctAnswer.columnIndexes = {1, 4, 0, 3, 5, 0, 3, 4, 1, 4, 0};
  correctAnswer.values = {43.12, 4.32, 1.43, 65.45, 0.65, 24.6, 12.75, 20.91, 5.75, 4.5, 20.48};

  ASSERT_EQ(z.numberOfRows, correctAnswer.numberOfRows);
  ASSERT_EQ(z.numberOfColumns, correctAnswer.numberOfColumns);
  ASSERT_EQ(z.pointers, correctAnswer.pointers);
  ASSERT_EQ(z.columnIndexes, correctAnswer.columnIndexes);
  ASSERT_EQ(z.values.size(), correctAnswer.values.size());
  // ASSERT_EQ(z.values, correctAnswer.values);
  for (size_t i = 0; i < correctAnswer.values.size(); ++i) {
    double t = correctAnswer.values[i] - z.values[i];
    ASSERT_NEAR(0.0f, t, 1e-6);
  }
}

TEST(Safarov_N_SparseMatMultCRS, TestFive) {
  // Create data
  SparseMatrixCRS x;
  x.numberOfRows = 3;
  x.numberOfColumns = 4;
  x.pointers = {0, 1, 2, 3};
  x.columnIndexes = {0, 2, 3};
  x.values = {7.2, 10.5, 13.1};

  SparseMatrixCRS y;
  y.numberOfRows = 4;
  y.numberOfColumns = 3;
  y.pointers = {0, 1, 1, 2, 3};
  y.columnIndexes = {2, 0, 1};
  y.values = {8.3, 5.7, 12.4};
  SparseMatrixCRS z;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&x));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&y));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&z));

  // Create Task
  SparseMatrixMultiplicationCRS taskSequential(taskDataSeq);
  ASSERT_EQ(taskSequential.validation(), true);
  ASSERT_EQ(taskSequential.pre_processing(), true);
  ASSERT_EQ(taskSequential.run(), true);
  ASSERT_EQ(taskSequential.post_processing(), true);

  SparseMatrixCRS correctAnswer;
  correctAnswer.numberOfRows = 3;
  correctAnswer.numberOfColumns = 3;
  correctAnswer.pointers = {0, 1, 2, 3};
  correctAnswer.columnIndexes = {2, 0, 1};
  correctAnswer.values = {59.76, 59.85, 162.44};

  ASSERT_EQ(z.numberOfRows, correctAnswer.numberOfRows);
  ASSERT_EQ(z.numberOfColumns, correctAnswer.numberOfColumns);
  ASSERT_EQ(z.pointers, correctAnswer.pointers);
  ASSERT_EQ(z.columnIndexes, correctAnswer.columnIndexes);
  ASSERT_EQ(z.values.size(), correctAnswer.values.size());
  // ASSERT_EQ(z.values, correctAnswer.values);
  for (size_t i = 0; i < correctAnswer.values.size(); ++i) {
    double t = correctAnswer.values[i] - z.values[i];
    ASSERT_NEAR(0.0f, t, 1e-6);
  }
}
