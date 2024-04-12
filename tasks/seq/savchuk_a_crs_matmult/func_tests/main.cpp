// Copyright 2024 Savchuk Anton
#include <gtest/gtest.h>

#include <vector>

#include "seq/savchuk_a_crs_matmult/include/crs_matmult_seq.hpp"

TEST(savchuk_a_crs_matmult, test_sizes) {
  size_t n1 = 4;
  size_t m1 = 6;
  size_t n2 = 6;
  size_t m2 = 2;

  // Create data
  std::vector<double> in1(n1 * m1);

  std::vector<double> in2(n2 * m2);

  std::vector<double> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SavchukCRSMatMult savchukCRSMatMult(taskDataSeq);
  ASSERT_EQ(savchukCRSMatMult.validation(), true);
}

TEST(savchuk_a_crs_matmult, test_sizes2) {
  size_t n1 = 2;
  size_t m1 = 3;
  size_t n2 = 4;
  size_t m2 = 5;

  // Create data
  std::vector<double> in1(n1 * m1);

  std::vector<double> in2(n2 * m2);

  std::vector<double> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SavchukCRSMatMult savchukCRSMatMult(taskDataSeq);
  ASSERT_FALSE(savchukCRSMatMult.validation());
}

TEST(savchuk_a_crs_matmult, test_multy_correct) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;

  // Create data
  std::vector<double> in1{4, 0, 0, 0, 0, 1, 0, 2, 0};
  std::vector<double> in2{9, 1, 0, 0, 0, 7, 3, 0, 0};
  std::vector<double> out(n1 * m2);
  std::vector<double> test{36, 4, 0, 3, 0, 0, 0, 0, 14};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SavchukCRSMatMult savchukCRSMatMult(taskDataSeq);
  savchukCRSMatMult.validation();
  savchukCRSMatMult.pre_processing();
  savchukCRSMatMult.run();
  savchukCRSMatMult.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i] == test[i]) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);
}

TEST(savchuk_a_crs_matmult, inverse_matrix) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;
  // Create data
  std::vector<double> in1{4, 0, 0, 0, 2, 1, 0, 2, 0};
  std::vector<double> in2{0.25, 0, 0, 0, 0, 0.5, 0, 1, -1};
  std::vector<double> out(n1 * m2);
  std::vector<double> identity{1, 0, 0, 0, 1, 0, 0, 0, 1};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);
  // Create Task
  SavchukCRSMatMult savchukCRSMatMult(taskDataSeq);
  savchukCRSMatMult.validation();
  savchukCRSMatMult.pre_processing();
  savchukCRSMatMult.run();
  savchukCRSMatMult.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i] == identity[i]) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);
}

TEST(savchuk_a_crs_matmult, zero_matrix) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;
  // Create data
  std::vector<double> in1{0, 2, 0, 0, 0, 6, 0, 2, 0};

  std::vector<double> in2(n2 * m2, 0);

  std::vector<double> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SavchukCRSMatMult savchukCRSMatMult(taskDataSeq);
  savchukCRSMatMult.validation();
  savchukCRSMatMult.pre_processing();
  savchukCRSMatMult.run();
  savchukCRSMatMult.post_processing();

  size_t m = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i] == 0.0) {
      m++;
    }
  }

  ASSERT_EQ(m, n1 * m2);
}