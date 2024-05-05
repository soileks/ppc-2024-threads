// Copyright 2024 Saratova Marina
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <iostream>
#include <vector>

#include "tbb/saratova_m_mult_matrix_fox/include/ops_tbb.hpp"

using namespace saratova_tbb;

TEST(Saratova_M_Mult_Matrix_Fox, Validation_Test) {
  size_t n = 2;
  std::vector<double> in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> in2{1.0, 0.0, 0.0, 1.0, 2.0};
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(out.size());

  // Create Task
  SaratovaTaskTbb saratovaTaskTbb(taskDataTbb);
  ASSERT_EQ(saratovaTaskTbb.validation(), false);
}

TEST(Saratova_M_Mult_Matrix_Fox, Identity_Mult_On_Another_Matrix) {
  size_t n = 2;
  std::vector<double> in1{0.0, 1.0, 1.0, 0.0};
  std::vector<double> in2{24.0, 45.0, -31.0, 10.0};
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(out.size());

  // Create Task
  SaratovaTaskTbb saratovaTaskTbb(taskDataTbb);
  ASSERT_EQ(saratovaTaskTbb.validation(), true);
  saratovaTaskTbb.pre_processing();
  saratovaTaskTbb.run();
  saratovaTaskTbb.post_processing();
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(out[i * n + j], in2[((n - i - 1)) * n + j]);
    }
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, Identity_Mult_On_Matrix) {
  size_t n = 2;
  std::vector<double> in1{1.0, 0.0, 0.0, 1.0};
  std::vector<double> in2{24.0, 45.0, -31.0, 10.0};
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(out.size());

  // Create Task
  SaratovaTaskTbb saratovaTaskTbb(taskDataTbb);
  ASSERT_EQ(saratovaTaskTbb.validation(), true);
  saratovaTaskTbb.pre_processing();
  saratovaTaskTbb.run();
  saratovaTaskTbb.post_processing();
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(out[i], in2[i]);
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, Matrix_Mult_On_Identity) {
  size_t n = 2;
  std::vector<double> in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> in2{1.0, 0.0, 0.0, 1.0};
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(out.size());

  // Create Task
  SaratovaTaskTbb saratovaTaskTbb(taskDataTbb);
  ASSERT_EQ(saratovaTaskTbb.validation(), true);
  saratovaTaskTbb.pre_processing();
  saratovaTaskTbb.run();
  saratovaTaskTbb.post_processing();
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(out[i], in1[i]);
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, Matrix_Mult_On_Another_Identity) {
  size_t n = 2;
  std::vector<double> in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> in2{0.0, 1.0, 1.0, 0.0};
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(out.size());

  // Create Task
  SaratovaTaskTbb saratovaTaskTbb(taskDataTbb);
  ASSERT_EQ(saratovaTaskTbb.validation(), true);
  saratovaTaskTbb.pre_processing();
  saratovaTaskTbb.run();
  saratovaTaskTbb.post_processing();
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(out[i * n + j], in1[i * n + (n - j - 1)]);
    }
  }
}