// Copyright 2024 Benduyzhko Tatiana
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "omp/benduyzhko_t_shell_batcher/include/ops_omp.hpp"

TEST(benduyzhko_t_shell_batcher_omp, Test8Nums) {
  std::vector<int> arr = {-1, 2, 1, 4, 8, 6, 5, 10};
  std::vector<int> out = arr;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->inputs_count.emplace_back(out.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  BenduyzhkoOMP task(taskData);
  ASSERT_EQ(task.validation(), true);
  task.pre_processing();
  task.run();
  task.post_processing();

  std::sort(arr.begin(), arr.end());
  ASSERT_EQ(arr, out);
}

TEST(benduyzhko_t_shell_batcher_omp, Test16Nums) {
  std::vector<int> arr = {-1, 2, 1, 4, 3, 0, 7, 9, 8, 6, 5, 10, -5, 12, -9, 11};
  std::vector<int> out = arr;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->inputs_count.emplace_back(out.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  BenduyzhkoOMP task(taskData);
  ASSERT_EQ(task.validation(), true);
  task.pre_processing();
  task.run();
  task.post_processing();

  std::sort(arr.begin(), arr.end());
  ASSERT_EQ(arr, out);
}

TEST(benduyzhko_t_shell_batcher_omp, Test32NumsRandom) {
  int n = 32;
  std::vector<int> arr(n);
  BenduyzhkoOMP::get_random_numbers(arr.data(), n);
  std::vector<int> out = arr;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->inputs_count.emplace_back(out.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  BenduyzhkoOMP task(taskData);
  ASSERT_EQ(task.validation(), true);
  task.pre_processing();
  task.run();
  task.post_processing();

  std::sort(arr.begin(), arr.end());
  ASSERT_EQ(arr, out);
}

TEST(benduyzhko_t_shell_batcher_omp, Test128NumsRandom) {
  int n = 128;
  std::vector<int> arr(n);
  BenduyzhkoOMP::get_random_numbers(arr.data(), n, -n, n);
  std::vector<int> out = arr;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->inputs_count.emplace_back(out.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  BenduyzhkoOMP task(taskData);
  ASSERT_EQ(task.validation(), true);
  task.pre_processing();
  task.run();
  task.post_processing();

  std::sort(arr.begin(), arr.end());
  ASSERT_EQ(arr, out);
}

TEST(benduyzhko_t_shell_batcher_omp, Test512NumsRandom) {
  int n = 512;
  std::vector<int> arr(n);
  BenduyzhkoOMP::get_random_numbers(arr.data(), n, -n, n);
  std::vector<int> out = arr;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->inputs_count.emplace_back(out.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  BenduyzhkoOMP task(taskData);
  ASSERT_EQ(task.validation(), true);
  task.pre_processing();
  task.run();
  task.post_processing();

  std::sort(arr.begin(), arr.end());
  ASSERT_EQ(arr, out);
}
