// Copyright 2024 Balyasov Ivan
#include <gtest/gtest.h>

#include <vector>

#include "omp/balyasov_i_sort_simple_merge_omp/include/ops_seq.hpp"

TEST(balyasov_i_sort_simple_merge_omp, test_three_values) {
  // Create data
  std::vector<int> in = {8, 6, 2};
  std::vector<int> answer = {2, 6, 8};
  std::vector<int> out(3);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  RadixSortTaskOMP Task(taskDataSeq);
  ASSERT_EQ(Task.validation(), true);
  Task.pre_processing();
  Task.run();
  Task.post_processing();
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], answer[i]);
  }
}

TEST(balyasov_i_sort_simple_merge_omp, test_eleven_values) {
  // Create data
  std::vector<int> in = {7, 9, 3, 2, 1, 30, 11, 8, 10, 16, 15};
  std::vector<int> answer = {1, 2, 3, 7, 8, 9, 10, 11, 15, 16, 30};
  std::vector<int> out(11);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  RadixSortTaskOMP Task(taskDataSeq);
  ASSERT_EQ(Task.validation(), true);
  Task.pre_processing();
  Task.run();
  Task.post_processing();
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], answer[i]);
  }
}

TEST(balyasov_i_sort_simple_merge_omp, test_with_duplicate) {
  // Create data
  std::vector<int> in = {1, 10, 9, 9, 8, 6, 5, 4, 3, 2, 1, 8, 6};
  std::vector<int> answer = {1, 1, 2, 3, 4, 5, 6, 6, 8, 8, 9, 9, 10};
  std::vector<int> out(13);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  RadixSortTaskOMP Task(taskDataSeq);
  ASSERT_EQ(Task.validation(), true);
  Task.pre_processing();
  Task.run();
  Task.post_processing();
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], answer[i]);
  }
}

TEST(balyasov_i_sort_simple_merge_omp, sort_random_vector) {
  // Create data
  std::vector<int> in = {64, 3, 99, 63, 57, 71, 53, 76, 85, 42, 58, 51, 63, 6, 75, 56, 84, 8, 6, 12};
  std::vector<int> answer = {3, 6, 6, 8, 12, 42, 51, 53, 56, 57, 58, 63, 63, 64, 71, 75, 76, 84, 85, 99};
  std::vector<int> out(20);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  RadixSortTaskOMP Task(taskDataSeq);
  ASSERT_EQ(Task.validation(), true);
  Task.pre_processing();
  Task.run();
  Task.post_processing();
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], answer[i]);
  }
}

TEST(balyasov_i_sort_simple_merge_omp, validation_test) {
  // Create data
  std::vector<int> in = {3, 2, 1};
  std::vector<int> answer = {1, 2, 3};
  std::vector<int> out(4);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  RadixSortTaskOMP Task(taskDataSeq);
  ASSERT_NE(Task.validation(), true);
}