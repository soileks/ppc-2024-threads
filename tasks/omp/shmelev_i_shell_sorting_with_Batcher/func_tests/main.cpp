// Copyright 2024 Shmelev Ivan
#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "omp/shmelev_i_shell_sorting_with_Batcher/include/ops_seq.hpp"

using namespace shmelev_omp;

TEST(shmelev_i_shell_sorting_with_Batcher, validation_check) {
  std::vector<int> input_array = {10, -1, 0};
  std::vector<int> sorted_array(3);
  std::vector<int> expected_array = {-1, 0, 10};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_array));
  taskDataPar->inputs_count.emplace_back(input_array.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(expected_array.size() + 1);

  // Create Task
  ShmelevTaskOmp testTaskOmp(taskDataPar);
  ASSERT_EQ(testTaskOmp.validation(), false);
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_array1) {
  std::vector<int> input_array = {12, 0, -32};
  std::vector<int> sorted_array(3);
  std::vector<int> expected_array = {-32, 0, 12};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskOmp testTaskOmp(taskDataPar);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(sorted_array[i], expected_array[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_array2) {
  std::vector<int> input_array = {10, -1, 0};
  std::vector<int> sorted_array(3);
  std::vector<int> expected_array = {-1, 0, 10};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskOmp testTaskOmp(taskDataPar);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(sorted_array[i], expected_array[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_array3) {
  std::vector<int> input_array = {1, 0, -2};
  std::vector<int> sorted_array(3);
  std::vector<int> expected_array = {-2, 0, 1};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskOmp testTaskOmp(taskDataPar);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(sorted_array[i], expected_array[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_array4) {
  std::vector<int> input_array = {123, 0, -322};
  std::vector<int> sorted_array(3);
  std::vector<int> expected_array = {-322, 0, 123};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_array));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataPar->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskOmp testTaskOmp(taskDataPar);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(sorted_array[i], expected_array[i]);
  }
}