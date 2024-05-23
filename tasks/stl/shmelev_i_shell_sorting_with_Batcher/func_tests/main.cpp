// Copyright 2024 Shmelev Ivan
#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "stl/shmelev_i_shell_sorting_with_Batcher/include/ops_stl.hpp"

using namespace shmelev_stl;

TEST(shmelev_i_shell_sorting_with_Batcher, validation_check) {
  std::vector<int> array = {10, -1, 0};
  std::vector<int> sorted_array(3);
  std::vector<int> expected = {-1, 0, 10};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataPar->inputs_count.emplace_back(array.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(expected.size() + 1);

  // Create Task
  ShmelevTaskSTL testTaskStl(taskDataPar);
  ASSERT_EQ(testTaskStl.validation(), false);
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_small_array) {
  std::vector<int> array = {12, 0, -32};
  std::vector<int> sorted_array(3);
  std::vector<int> expected = {-32, 0, 12};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskSTL testTaskStl(taskDataPar);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_another_small_array) {
  std::vector<int> array = {60, -11, 234, 10};
  std::vector<int> sorted_array(4);
  std::vector<int> expected = {-11, 10, 60, 234};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskSTL testTaskStl(taskDataPar);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_medium_array) {
  std::vector<int> array = {17, 23, 8, 14, 29, 5, 11, 20, 26, 2, 32, 35, 10, 38, 41, 47};
  std::vector<int> sorted_array(16);
  std::vector<int> expected = {2, 5, 8, 10, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 47};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskSTL testTaskStl(taskDataPar);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();
  for (int i = 0; i < 16; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting__another_medium_array) {
  std::vector<int> array = {23, 17, 32, 14, 29, 11, 5, 2, 26, 20, 8, 35, 10, 38, 47, 41};
  std::vector<int> sorted_array(32);
  std::vector<int> expected = {2, 5, 8, 10, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 47};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskSTL testTaskStl(taskDataPar);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();
  for (int i = 0; i < 30; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}