// Copyright 2024 Vasilev Ivan
#include <gtest/gtest.h>

#include <vector>

#include "tasks\tbb\Vasilev_ivan_shell_sort\include\ops_tbb.hpp"

TEST(vasilev_i_sort_tbb, Sort_2) {
  std::vector<int> arr = {2, 1};
  std::vector<int> rez(2);
  std::vector<int> expected = {1, 2};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&arr));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&rez));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  VasilevTaskTbb testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(rez, expected);
}

TEST(vasilev_i_sort_tbb, Sort_4) {
  std::vector<int> arr = {3, 2, 4, 1};
  std::vector<int> rez(4);
  std::vector<int> expected = {1, 2, 3, 4};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&arr));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&rez));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  VasilevTaskTbb testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(rez, expected);
}

TEST(vasilev_i_sort_tbb, Sort_8) {
  std::vector<int> arr = {5, 3, 8, 6, 2, 7, 1, 4};
  std::vector<int> rez(8);
  std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&arr));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&rez));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  VasilevTaskTbb testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(rez, expected);
}

TEST(vasilev_i_sort_tbb, Sort_16) {
  std::vector<int> arr = {5, 8, 11, 6, 12, 16, 9, 3, 13, 2, 7, 4, 10, 1, 14, 15};
  std::vector<int> rez(16);
  std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&arr));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&rez));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  VasilevTaskTbb testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(rez, expected);
}

TEST(vasilev_i_sort_tbb, Sort_32) {
  std::vector<int> arr = {3,  24, 15, 10, 32, 1, 25, 28, 2,  8,  20, 9,  11, 30, 12, 6,
                          22, 5,  18, 4,  31, 7, 13, 17, 23, 29, 16, 14, 27, 26, 19, 21};
  std::vector<int> rez(32);
  std::vector<int> expected = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
                               17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&arr));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&rez));
  taskDataSeq->outputs_count.emplace_back(1);

  // Create Task
  VasilevTaskTbb testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(rez, expected);
}
