// Copyright 2024 Vinokurov Ivan
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "tbb/vinokurov_i_simpson_method/include/ops_tbb.hpp"

using namespace vinokurovIvanTBB;

TEST(vinokurov_i_simpson_method_tbb, test_linear) {
  const int a = 0;
  const int b = 10;
  const int c = 0;
  const int d = 10;
  const int n = 100;

  const double result = 90;

  func fn = fn_linear;

  // Create data

  std::vector<int> in{a, b, c, d, n};
  std::vector<double> out(1, result);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  SimpsonMethodTBB testTaskTBB(taskDataTBB, fn);
  ASSERT_EQ(testTaskTBB.validation(), true);
  testTaskTBB.pre_processing();
  testTaskTBB.run();
  testTaskTBB.post_processing();
  ASSERT_LE(result - out[0], 0.01);
}

TEST(vinokurov_i_simpson_method_omp, test_exp) {
  const int a = 0;
  const int b = 10;
  const int c = 0;
  const int d = 10;
  const int n = 100;

  double result = 27.18;

  func fn = fn_exp;

  // Create data
  std::vector<int> in{a, b, c, d, n};
  std::vector<double> out(1, result);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  SimpsonMethodTBB testTaskTBB(taskDataTBB, fn);
  ASSERT_EQ(testTaskTBB.validation(), true);
  testTaskTBB.pre_processing();
  testTaskTBB.run();
  testTaskTBB.post_processing();
  ASSERT_LE(result - out[0], 0.01);
}

TEST(vinokurov_i_simpson_method_omp, test_trig) {
  const int a = 6;
  const int b = 18;
  const int c = 6;
  const int d = 18;
  const int n = 100;

  double result = 0.04;

  func fn = fn_trig;

  // Create data
  std::vector<int> in{a, b, c, d, n};
  std::vector<double> out(1, result);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  SimpsonMethodTBB testTaskTBB(taskDataTBB, fn);
  ASSERT_EQ(testTaskTBB.validation(), true);
  testTaskTBB.pre_processing();
  testTaskTBB.run();
  testTaskTBB.post_processing();
  ASSERT_LE(result - out[0], 0.01);
}

TEST(vinokurov_i_simpson_method_omp, test_mul) {
  const int a = 0;
  const int b = 10;
  const int c = 0;
  const int d = 10;
  const int n = 100;

  double result = 16666.66;

  func fn = fn_mul;

  // Create data
  std::vector<int> in{a, b, c, d, n};
  std::vector<double> out(1, result);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  SimpsonMethodTBB testTaskTBB(taskDataTBB, fn);
  ASSERT_EQ(testTaskTBB.validation(), true);
  testTaskTBB.pre_processing();
  testTaskTBB.run();
  testTaskTBB.post_processing();
  ASSERT_LE(result - out[0], 0.01);
}

TEST(vinokurov_i_simpson_method_omp, test_other) {
  const int a = 0;
  const int b = 10;
  const int c = 0;
  const int d = 10;
  const int n = 50;

  double result = 5833.33;

  func fn = fn_other;

  // Create data
  std::vector<int> in{a, b, c, d, n};
  std::vector<double> out(1, result);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  SimpsonMethodTBB testTaskTBB(taskDataTBB, fn);
  ASSERT_EQ(testTaskTBB.validation(), true);
  testTaskTBB.pre_processing();
  testTaskTBB.run();
  testTaskTBB.post_processing();
  ASSERT_LE(result - out[0], 0.01);
}
