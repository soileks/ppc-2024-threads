// Copyright 2024 Lysanova Julia
#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "omp/lysanova_j_matr_fox/include/ops_omp.hpp"

namespace me_omp = lysanova_j_matr_fox_omp;

TEST(lysanova_j_matr_fox_omp, AE) {
  size_t n = 2;
  std::vector<double> A{42.0, 54.0, -13.0, 0.0};
  std::vector<double> B{1.0, 0.0, 0.0, 1.0};
  std::vector<double> C(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataOmp->inputs_count.emplace_back(A.size());
  taskDataOmp->inputs_count.emplace_back(B.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataOmp->outputs_count.emplace_back(C.size());

  // Create Task
  LysanovaTaskOmp lysanovaTaskOmp(taskDataOmp);
  ASSERT_EQ(lysanovaTaskOmp.validation(), true);
  lysanovaTaskOmp.pre_processing();
  lysanovaTaskOmp.run();
  lysanovaTaskOmp.post_processing();
  for (size_t i = 0; i < A.size(); i++) {
    EXPECT_DOUBLE_EQ(C[i], A[i]);
  }
}

TEST(lysanova_j_matr_fox_omp, AEsd) {
  size_t n = 2;
  std::vector<double> A{42.0, 54.0, -13.0, 0.0};
  std::vector<double> B{0.0, 1.0, 1.0, 0.0};
  std::vector<double> C(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataOmp->inputs_count.emplace_back(A.size());
  taskDataOmp->inputs_count.emplace_back(B.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataOmp->outputs_count.emplace_back(C.size());

  // Create Task
  LysanovaTaskOmp lysanovaTaskOmp(taskDataOmp);
  ASSERT_EQ(lysanovaTaskOmp.validation(), true);
  lysanovaTaskOmp.pre_processing();
  lysanovaTaskOmp.run();
  lysanovaTaskOmp.post_processing();
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(C[i * n + j], A[i * n + (n - j - 1)]);
    }
  }
}

TEST(lysanova_j_matr_fox_omp, EA) {
  size_t n = 8;
  std::vector<double> C(n * n);
  std::vector<double> B(n * n);
  std::vector<double> A(n * n);
  me_omp::RandomFillMatrix(B.data(), B.size());
  me_omp::EMatrix(A.data(), n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataOmp->inputs_count.emplace_back(A.size());
  taskDataOmp->inputs_count.emplace_back(B.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataOmp->outputs_count.emplace_back(C.size());

  // Create Task
  LysanovaTaskOmp lysanovaTaskOmp(taskDataOmp);
  ASSERT_EQ(lysanovaTaskOmp.validation(), true);
  lysanovaTaskOmp.pre_processing();
  lysanovaTaskOmp.run();
  lysanovaTaskOmp.post_processing();
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(C[i], B[i]);
  }
}

TEST(lysanova_j_matr_fox_omp, EsdA) {
  size_t n = 8;
  std::vector<double> C(n * n);
  std::vector<double> B(n * n);
  std::vector<double> A(n * n);
  me_omp::RandomFillMatrix(B.data(), B.size());
  me_omp::EsdMatrix(A.data(), n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataOmp->inputs_count.emplace_back(A.size());
  taskDataOmp->inputs_count.emplace_back(B.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataOmp->outputs_count.emplace_back(C.size());

  // Create Task
  LysanovaTaskOmp lysanovaTaskOmp(taskDataOmp);
  ASSERT_EQ(lysanovaTaskOmp.validation(), true);
  lysanovaTaskOmp.pre_processing();
  lysanovaTaskOmp.run();
  lysanovaTaskOmp.post_processing();
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(C[i * n + j], B[((n - i - 1)) * n + j]);
    }
  }
}

TEST(lysanova_j_matr_fox_omp, kAE) {
  size_t n = 8;
  double k = 42.0;
  std::vector<double> C(n * n);
  std::vector<double> B(n * n);
  std::vector<double> A(n * n);
  me_omp::RandomFillMatrix(A.data(), A.size());
  me_omp::EMatrix(B.data(), n, k);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataOmp->inputs_count.emplace_back(A.size());
  taskDataOmp->inputs_count.emplace_back(B.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataOmp->outputs_count.emplace_back(C.size());

  // Create Task
  LysanovaTaskOmp lysanovaTaskOmp(taskDataOmp);
  ASSERT_EQ(lysanovaTaskOmp.validation(), true);
  lysanovaTaskOmp.pre_processing();
  lysanovaTaskOmp.run();
  lysanovaTaskOmp.post_processing();
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(C[i], k * A[i]);
  }
}
