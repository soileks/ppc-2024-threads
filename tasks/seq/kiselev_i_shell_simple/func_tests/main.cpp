// Copyright 2024 Kiselev Igor
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "seq/kiselev_i_shell_simple/include/shell_simple.hpp"

TEST(kiselev_i_shell_simple_seq, check_10_size) {
  std::vector<int> inputArray = {3, 5, 6, 4, 1, 2, 0, 9, 8, 7};
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  KiselevTaskSequential kiselevTaskSequential(hoareSortSequential);
  ASSERT_TRUE(kiselevTaskSequential.validation());
  kiselevTaskSequential.pre_processing();
  kiselevTaskSequential.run();
  kiselevTaskSequential.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(kiselev_i_shell_simple_seq, check_100_size_reverse) {
  const int count = 100;

  // Create data
  std::vector<int> in(count, 0);
  std::vector<int> out(count, 0);
  std::vector<int> res(count, 0);
  for (int i = 0; i < count; i++) {
    in[i] = count - 1 - i;
    res[i] = i;
  }
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  KiselevTaskSequential kiselevTaskSequential(taskDataSeq);
  ASSERT_EQ(kiselevTaskSequential.validation(), true);
  ASSERT_EQ(kiselevTaskSequential.pre_processing(), true);
  ASSERT_EQ(kiselevTaskSequential.run(), true);
  ASSERT_EQ(kiselevTaskSequential.post_processing(), true);
  ASSERT_EQ(res, out);
}

TEST(kiselev_i_shell_simple_seq, check_100_size_sorted) {
  const int count = 100;

  // Create data
  std::vector<int> in(count, 0);
  std::vector<int> out(count, 0);
  std::vector<int> res(count, 0);
  for (int i = 0; i < count; i++) {
    in[i] = i;
    res[i] = i;
  }
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  KiselevTaskSequential kiselevTaskSequential(taskDataSeq);
  ASSERT_EQ(kiselevTaskSequential.validation(), true);
  ASSERT_EQ(kiselevTaskSequential.pre_processing(), true);
  ASSERT_EQ(kiselevTaskSequential.run(), true);
  ASSERT_EQ(kiselevTaskSequential.post_processing(), true);
  ASSERT_EQ(res, out);
}

TEST(kiselev_i_shell_simple_seq, check_100_size_bias) {
  const int count = 100;
  const int bias = 50;

  // Create data
  std::vector<int> in(count, 0);
  std::vector<int> out(count, 0);
  std::vector<int> res(count, 0);
  for (int i = 0; i < count; i++) {
    in[i] = count - 1 - i + bias;
    res[i] = i + bias;
  }
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  KiselevTaskSequential kiselevTaskSequential(taskDataSeq);
  ASSERT_EQ(kiselevTaskSequential.validation(), true);
  ASSERT_EQ(kiselevTaskSequential.pre_processing(), true);
  ASSERT_EQ(kiselevTaskSequential.run(), true);
  ASSERT_EQ(kiselevTaskSequential.post_processing(), true);
  ASSERT_EQ(res, out);
}

TEST(kiselev_i_shell_simple_seq, check_100_size_random) {
  const int count = 100;

  // Create data
  std::vector<int> in(count, 0);
  std::vector<int> out(count, 0);
  std::vector<int> res(count, 0);
  for (int i = 0; i < count; i++) {
    int a = std::rand() % 100;
    in[i] = a;
    res[i] = a;
  }
  sort(res.begin(), res.end());
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  KiselevTaskSequential kiselevTaskSequential(taskDataSeq);
  ASSERT_EQ(kiselevTaskSequential.validation(), true);
  ASSERT_EQ(kiselevTaskSequential.pre_processing(), true);
  ASSERT_EQ(kiselevTaskSequential.run(), true);
  ASSERT_EQ(kiselevTaskSequential.post_processing(), true);
  ASSERT_EQ(res, out);
}

// TEST(kiselev_i_shell_simple_seq, check_incorrect_input) {
//  const int count = 2;

// Create data
//  std::vector<std::vector<int>> in = {{2, 1}, {4, 3}};
//  std::vector<int> out(count, 0);
//  std::vector<int> res = {1, 2, 3, 4};
// Create TaskData
//  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
//  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
//  taskDataSeq->inputs_count.emplace_back(in.size());
//  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
//  taskDataSeq->outputs_count.emplace_back(out.size());

// Create Task
//  KiselevTaskSequential kiselevTaskSequential(taskDataSeq);
//  ASSERT_EQ(kiselevTaskSequential.validation(), false);
// }
