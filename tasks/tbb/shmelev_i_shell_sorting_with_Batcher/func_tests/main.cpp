// Copyright 2024 Shmelev Ivan
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <iostream>
#include <vector>

#include "tbb/shmelev_i_shell_sorting_with_Batcher/include/ops_tbb.hpp"

using namespace shmelev_tbb;

TEST(shmelev_i_shell_sorting_with_Batcher, validation_check) {
  std::vector<int> array = {10, -1, 0};
  std::vector<int> sorted_array(3);
  std::vector<int> expected = {-1, 0, 10};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataSeq->inputs_count.emplace_back(array.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataSeq->outputs_count.emplace_back(expected.size() + 1);

  // Create Task
  ShmelevTaskTbb testTaskTbb(taskDataSeq);
  ASSERT_EQ(testTaskTbb.validation(), false);
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_small_array) {
  std::vector<int> array = {12, 0, -32};
  std::vector<int> sorted_array(3);
  std::vector<int> expected = {-32, 0, 12};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskTbb testTaskTbb(taskDataSeq);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_another_small_array) {
  std::vector<int> array = {60, -11, 234, 10};
  std::vector<int> sorted_array(4);
  std::vector<int> expected = {-11, 10, 60, 234};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskTbb testTaskTbb(taskDataSeq);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_medium_array) {
  std::vector<int> array = {131, -111, 34, -3, 0};
  std::vector<int> sorted_array(5);
  std::vector<int> expected = {-111, -3, 0, 34, 131};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskTbb testTaskTbb(taskDataSeq);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}

TEST(shmelev_i_shell_sorting_with_Batcher, Sorting_another_array) {
  std::vector<int> array = {1, 0};
  std::vector<int> sorted_array(2);
  std::vector<int> expected = {0, 1};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&array));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&sorted_array));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  ShmelevTaskTbb testTaskTbb(taskDataSeq);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(sorted_array[i], expected[i]);
  }
}