// Copyright 2024 Mironov Ilya
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "tbb/mironov_i_sparse_crs/include/ops_tbb.hpp"

TEST(mironov_i_sparse_crs_tbb, Test1Static) {
  std::vector<double> C(3 * 3, 0.0);
  std::vector<double> A = {2, 0, 2, 0, 0, 1, 0, 2, 0, 0, 3, 0};
  std::vector<double> B = {2, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 3};
  std::vector<double> res = {4, 2, 4, 0, 0, 6, 0, 3, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskData->inputs_count.emplace_back(3);
  taskData->inputs_count.emplace_back(A.size() / 3);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskData->inputs_count.emplace_back(4);
  taskData->inputs_count.emplace_back(A.size() / 4);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  // Create Task
  MironovITBB testTask(taskData);
  ASSERT_EQ(testTask.validation(), true);
  testTask.pre_processing();
  testTask.run();
  testTask.post_processing();

  for (size_t i = 0; i < C.size(); i++) {
    EXPECT_DOUBLE_EQ(res[i], C[i]);
  }
}

TEST(mironov_i_sparse_crs_tbb, Test2Static) {
  std::vector<double> C(3 * 3, 0.0);
  std::vector<double> A = {2, 0, 2, 0, 0, 1, 0, 2, -2, 0, 3, 0};
  std::vector<double> B = {2, 0, 0, 0, -3, 0, 0, 1, 4, 1, 2, 3};
  std::vector<double> res = {4, 2, 8, 2, 1, 6, -4, 3, 12};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskData->inputs_count.emplace_back(3);
  taskData->inputs_count.emplace_back(A.size() / 3);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskData->inputs_count.emplace_back(4);
  taskData->inputs_count.emplace_back(A.size() / 4);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  // Create Task
  MironovITBB testTask(taskData);
  ASSERT_EQ(testTask.validation(), true);
  testTask.pre_processing();
  testTask.run();
  testTask.post_processing();

  for (size_t i = 0; i < C.size(); i++) {
    EXPECT_DOUBLE_EQ(res[i], C[i]);
  }
}

TEST(mironov_i_sparse_crs_tbb, TestEdin34) {
  std::vector<double> A = {2, 0, 2, 0, 0, 1, 0, 2, -2, 0, 3, 0};
  int n = 3;
  int m = A.size() / n;
  std::vector<double> B(m * m);
  MironovITBB::genrateEdMatrix(B.data(), m);
  std::vector<double> C(n * m, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskData->inputs_count.emplace_back(n);
  taskData->inputs_count.emplace_back(m);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskData->inputs_count.emplace_back(m);
  taskData->inputs_count.emplace_back(m);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  // Create Task
  MironovITBB testTask(taskData);
  ASSERT_EQ(testTask.validation(), true);
  testTask.pre_processing();
  testTask.run();
  testTask.post_processing();

  for (size_t i = 0; i < C.size(); i++) {
    EXPECT_DOUBLE_EQ(A[i], C[i]);
  }
}

TEST(mironov_i_sparse_crs_tbb, TestEdin43) {
  std::vector<double> A = {2, 0, 2, 0, 0, 1, 0, 2, -2, 0, 3, 0};
  int n = 4;
  int m = A.size() / n;
  std::vector<double> B(m * m);
  MironovITBB::genrateEdMatrix(B.data(), m);
  std::vector<double> C(n * m, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskData->inputs_count.emplace_back(n);
  taskData->inputs_count.emplace_back(m);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskData->inputs_count.emplace_back(m);
  taskData->inputs_count.emplace_back(m);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  // Create Task
  MironovITBB testTask(taskData);
  ASSERT_EQ(testTask.validation(), true);
  testTask.pre_processing();
  testTask.run();
  testTask.post_processing();

  for (size_t i = 0; i < C.size(); i++) {
    EXPECT_DOUBLE_EQ(A[i], C[i]);
  }
}

TEST(mironov_i_sparse_crs_tbb, TestRandom) {
  int n = 15;
  int m = 15;
  int k = 15;
  double ro = 0.3;
  std::vector<double> A(n * m, 0.0);
  MironovITBB::genrateSparseMatrix(A.data(), A.size(), ro);
  std::vector<double> B(m * k, 0.0);
  MironovITBB::genrateSparseMatrix(B.data(), B.size(), ro);
  std::vector<double> C(n * k, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskData->inputs_count.emplace_back(n);
  taskData->inputs_count.emplace_back(m);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskData->inputs_count.emplace_back(m);
  taskData->inputs_count.emplace_back(k);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  // Create Task
  MironovITBB testTask(taskData);
  ASSERT_EQ(testTask.validation(), true);
  testTask.pre_processing();
  testTask.run();
  testTask.post_processing();
}
