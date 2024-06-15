// Copyright 2024 Novostroev Ivan
#include <gtest/gtest.h>

#include <vector>

#include "stl/novostroev_i_task4_batcher_merge_sort/include/ops_stl.hpp"

TEST(novostroev_i_batcher_sort_func_stl, Test_size_10_stl) {
  const int count = 10;

  std::vector<int> vect = RandomVector(count);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  taskDataSeq->inputs_count.emplace_back(vect.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(result.size());

  BatcherMergeSortSTL testOmp(taskDataSeq);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(novostroev_i_batcher_sort_func_stl, Test_size_30_stl) {
  const int count = 30;

  std::vector<int> vect = RandomVector(count);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  taskDataSeq->inputs_count.emplace_back(vect.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(result.size());

  BatcherMergeSortSTL testOmp(taskDataSeq);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(novostroev_i_batcher_sort_func_stl, Test_small_size_stl) {
  const int count = 4;

  std::vector<int> vect = RandomVector(count);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  taskDataSeq->inputs_count.emplace_back(vect.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(result.size());

  BatcherMergeSortSTL testOmp(taskDataSeq);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(novostroev_i_batcher_sort_func_stl, Test_only_negative_numbers_stl) {
  std::vector<int> vect = {-998, -555, -100, -303, -20, -110, -340, -41, -9, -4, -199, -1};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  taskDataSeq->inputs_count.emplace_back(vect.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(result.size());

  BatcherMergeSortSTL testOmp(taskDataSeq);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(novostroev_i_batcher_sort_func_stl, Test_pre_sorted_stl) {
  std::vector<int> vect = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  taskDataSeq->inputs_count.emplace_back(vect.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(result.size());

  BatcherMergeSortSTL testOmp(taskDataSeq);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}
