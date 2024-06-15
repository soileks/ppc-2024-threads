// Copyright 2024 Shemiakina Alesia
#include <gtest/gtest.h>

#include <vector>

#include "omp/shemiakina_a_ShellSort/include/ops_omp.hpp"

void TestRandomVector(int count) {
  // Create data
  std::vector<int> in = ShellTaskOMP::give_random_vector(count, 1, 100);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOMP = std::make_shared<ppc::core::TaskData>();
  taskDataOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOMP->inputs_count.emplace_back(in.size());
  taskDataOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOMP->outputs_count.emplace_back(out.size());

  // Create Task
  ShellTaskOMP testTaskOMP(taskDataOMP);
  ASSERT_EQ(testTaskOMP.validation(), true);
  testTaskOMP.pre_processing();
  testTaskOMP.run();
  testTaskOMP.post_processing();
  ASSERT_TRUE(ShellTaskOMP::CheckSort(out));
}

TEST(shemiakina_a_ShellSort_OMP, Validation_Fail) {
  const int count = 10;

  // Create data
  std::vector<int> in1 = ShellTaskOMP::give_random_vector(count, 1, 100);
  std::vector<int> in2 = std::vector<int>(5);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOMP = std::make_shared<ppc::core::TaskData>();
  taskDataOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataOMP->inputs_count.emplace_back(in1.size());

  taskDataOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataOMP->inputs_count.emplace_back(in2.size());

  taskDataOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOMP->outputs_count.emplace_back(out.size());

  // Create Task
  ShellTaskOMP testTaskOMP(taskDataOMP);
  ASSERT_EQ(testTaskOMP.validation(), false);
}

TEST(shemiakina_a_ShellSort_OMP, Shell_Random_10) {
  const int count = 10;
  TestRandomVector(count);
}

TEST(shemiakina_a_ShellSort_OMP, Shell_Random_20) {
  const int count = 20;
  TestRandomVector(count);
}

TEST(shemiakina_a_ShellSort_OMP, Shell_Random_50) {
  const int count = 50;
  TestRandomVector(count);
}

TEST(shemiakina_a_ShellSort_OMP, Shell_Random_70) {
  const int count = 70;
  TestRandomVector(count);
}

TEST(shemiakina_a_ShellSort_OMP, Shell_Random_100) {
  const int count = 100;
  TestRandomVector(count);
}
