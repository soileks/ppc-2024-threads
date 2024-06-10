// Copyright 2024 Novostroev Ivan
#include <gtest/gtest.h>

#include <vector>

#include "tbb/novostroev_i_task3_batcher_merge_sort/include/ops_tbb.hpp"

// Helper function to generate a vector of integers sorted in reverse order
std::vector<int> getReverseSortedVector(int size) {
  std::vector<int> vec(size);
  std::iota(vec.rbegin(), vec.rend(), 0);
  return vec;
}

TEST(novostroev_i_batcher_sort_func_tbb, Test_SmallArray) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  std::vector<int> expected = vec;
  std::sort(expected.begin(), expected.end());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskData->inputs_count.emplace_back(vec.size());
  std::vector<int> out(vec.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto batcherMergeSortTask = std::make_shared<BatcherMergeSortTBB>(taskData);

  ASSERT_TRUE(batcherMergeSortTask->validation());
  batcherMergeSortTask->pre_processing();
  batcherMergeSortTask->run();
  batcherMergeSortTask->post_processing();

  ASSERT_EQ(expected, out);
}

TEST(novostroev_i_batcher_sort_func_tbb, Test_RandomArray) {
  std::vector<int> vec = RandomVector(100);
  std::vector<int> expected = vec;
  std::sort(expected.begin(), expected.end());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskData->inputs_count.emplace_back(vec.size());
  std::vector<int> out(vec.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto batcherMergeSortTask = std::make_shared<BatcherMergeSortTBB>(taskData);

  ASSERT_TRUE(batcherMergeSortTask->validation());
  batcherMergeSortTask->pre_processing();
  batcherMergeSortTask->run();
  batcherMergeSortTask->post_processing();

  ASSERT_EQ(expected, out);
}

TEST(novostroev_i_batcher_sort_func_tbb, Test_SortedArray) {
  std::vector<int> vec = RandomVector(100);
  std::sort(vec.begin(), vec.end());
  std::vector<int> expected = vec;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskData->inputs_count.emplace_back(vec.size());
  std::vector<int> out(vec.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto batcherMergeSortTask = std::make_shared<BatcherMergeSortTBB>(taskData);

  ASSERT_TRUE(batcherMergeSortTask->validation());
  batcherMergeSortTask->pre_processing();
  batcherMergeSortTask->run();
  batcherMergeSortTask->post_processing();

  ASSERT_EQ(expected, out);
}

// Test for an array with duplicate elements
TEST(novostroev_i_batcher_sort_func_tbb, Test_DuplicateElements) {
  std::vector<int> vec = {5, 3, 5, 2, 8, 3, 5, 9, 1, 5};
  std::vector<int> expected = vec;
  std::sort(expected.begin(), expected.end());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskData->inputs_count.emplace_back(vec.size());
  std::vector<int> out(vec.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto batcherMergeSortTask = std::make_shared<BatcherMergeSortTBB>(taskData);

  ASSERT_TRUE(batcherMergeSortTask->validation());
  batcherMergeSortTask->pre_processing();
  batcherMergeSortTask->run();
  batcherMergeSortTask->post_processing();

  ASSERT_EQ(expected, out);
}

// Test for a large array
TEST(novostroev_i_batcher_sort_func_tbb, Test_LargeArray) {
  std::vector<int> vec = RandomVector(10000);
  std::vector<int> expected = vec;
  std::sort(expected.begin(), expected.end());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskData->inputs_count.emplace_back(vec.size());
  std::vector<int> out(vec.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto batcherMergeSortTask = std::make_shared<BatcherMergeSortTBB>(taskData);

  ASSERT_TRUE(batcherMergeSortTask->validation());
  batcherMergeSortTask->pre_processing();
  batcherMergeSortTask->run();
  batcherMergeSortTask->post_processing();

  ASSERT_EQ(expected, out);
}

// Test for an array sorted in reverse order
TEST(novostroev_i_batcher_sort_func_tbb, Test_ReverseSortedArray) {
  std::vector<int> vec = getReverseSortedVector(100);
  std::vector<int> expected = vec;
  std::sort(expected.begin(), expected.end());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskData->inputs_count.emplace_back(vec.size());
  std::vector<int> out(vec.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto batcherMergeSortTask = std::make_shared<BatcherMergeSortTBB>(taskData);

  ASSERT_TRUE(batcherMergeSortTask->validation());
  batcherMergeSortTask->pre_processing();
  batcherMergeSortTask->run();
  batcherMergeSortTask->post_processing();

  ASSERT_EQ(expected, out);
}
