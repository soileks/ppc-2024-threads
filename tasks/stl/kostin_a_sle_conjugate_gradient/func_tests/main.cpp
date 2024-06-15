// Copyright 2024 Kostin Artem
#include <gtest/gtest.h>

#include <vector>

#include "stl/kostin_a_sle_conjugate_gradient/include/ops_stl.hpp"

using namespace KostinArtemSTL;

TEST(kostin_a_sle_conjugate_gradient_stl, Test_SLE_size_2) {
  int size = 2;

  // Create data
  std::vector<double> in_A = {2.0, 1.0, 1.0, 2.0};
  std::vector<double> in_b = {1.0, 3.0};
  std::vector<double> out(size, 0.0);
  std::vector<double> correct_answer = {-0.3333333, 1.6666666};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_A.data()));
  taskDataSTL->inputs_count.emplace_back(in_A.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_b.data()));
  taskDataSTL->inputs_count.emplace_back(in_b.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));
  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  ConjugateGradientMethodSTL testTaskSTL(taskDataSTL);
  ASSERT_EQ(testTaskSTL.validation(), true);
  testTaskSTL.pre_processing();
  testTaskSTL.run();
  testTaskSTL.post_processing();
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_LE(abs(correct_answer[i] - out[i]), 1e-6);
  }
}

TEST(kostin_a_sle_conjugate_gradient_stl, Test_SLE_size_5) {
  int size = 5;

  // Create data
  std::vector<double> in_A = {10.0, 15.0, 20.0, 25.0, 30.0, 15.0, 10.0, 15.0, 20.0, 25.0, 20.0, 15.0, 10.0,
                              15.0, 20.0, 25.0, 20.0, 15.0, 10.0, 15.0, 30.0, 25.0, 20.0, 15.0, 10.0};
  std::vector<double> in_b = {50.0, 55.0, 60.0, 65.0, 70.0};
  std::vector<double> out(size, 0.0);
  std::vector<double> correct_answer = {2.0, 0.0, 0.0, 0.0, 1.0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_A.data()));
  taskDataSTL->inputs_count.emplace_back(in_A.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_b.data()));
  taskDataSTL->inputs_count.emplace_back(in_b.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));
  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  ConjugateGradientMethodSTL testTaskSTL(taskDataSTL);
  ASSERT_EQ(testTaskSTL.validation(), true);
  testTaskSTL.pre_processing();
  testTaskSTL.run();
  testTaskSTL.post_processing();
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_LE(abs(correct_answer[i] - out[i]), 1e-6);
  }
}

TEST(kostin_a_sle_conjugate_gradient_stl, Test_generated_SLE_size_100) {
  int size = 100;

  // Create data
  std::vector<double> in_A = generateSPDMatrix(size, 100);
  std::vector<double> in_b = generatePDVector(size, 100);
  std::vector<double> out(size, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_A.data()));
  taskDataSTL->inputs_count.emplace_back(in_A.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_b.data()));
  taskDataSTL->inputs_count.emplace_back(in_b.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));
  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  ConjugateGradientMethodSTL testTaskSTL(taskDataSTL);
  ASSERT_EQ(testTaskSTL.validation(), true);
  testTaskSTL.pre_processing();
  testTaskSTL.run();
  testTaskSTL.post_processing();
  ASSERT_TRUE(check_solution(in_A, size, in_b, out, 1e-6));
}

TEST(kostin_a_sle_conjugate_gradient_stl, Test_generated_SLE_size_200) {
  int size = 200;

  // Create data
  std::vector<double> in_A = generateSPDMatrix(size, 100);
  std::vector<double> in_b = generatePDVector(size, 100);
  std::vector<double> out(size, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_A.data()));
  taskDataSTL->inputs_count.emplace_back(in_A.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_b.data()));
  taskDataSTL->inputs_count.emplace_back(in_b.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));
  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  ConjugateGradientMethodSTL testTaskSTL(taskDataSTL);
  ASSERT_EQ(testTaskSTL.validation(), true);
  testTaskSTL.pre_processing();
  testTaskSTL.run();
  testTaskSTL.post_processing();
  ASSERT_TRUE(check_solution(in_A, size, in_b, out, 1e-6));
}

TEST(kostin_a_sle_conjugate_gradient_stl, Test_generated_SLE_size_1) {
  int size = 1;

  // Create data
  std::vector<double> in_A = generateSPDMatrix(size, 100);
  std::vector<double> in_b = generatePDVector(size, 100);
  std::vector<double> out(size, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_A.data()));
  taskDataSTL->inputs_count.emplace_back(in_A.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_b.data()));
  taskDataSTL->inputs_count.emplace_back(in_b.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));
  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  ConjugateGradientMethodSTL testTaskSTL(taskDataSTL);
  ASSERT_EQ(testTaskSTL.validation(), true);
  testTaskSTL.pre_processing();
  testTaskSTL.run();
  testTaskSTL.post_processing();
  ASSERT_TRUE(check_solution(in_A, size, in_b, out, 1e-6));
}
