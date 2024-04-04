// Copyright 2024 Ivlev Alexander
#include <gtest/gtest.h>

#include <vector>

#include "seq/ivlev_a_convex_hull/include/ops_seq.hpp"

TEST(ivlev_a_convex_hull_sequential, one_component) {
  // Create data
  std::vector<std::vector<std::pair<size_t, size_t>>> in = {{(0, 0), (1, 1)}};
  std::vector<std::vector<std::pair<size_t, size_t>>> out{{}};
  std::vector<std::vector<std::pair<size_t, size_t>>> res = {{(0, 0), (1, 1)}};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(res, out);
}

TEST(ivlev_a_convex_hull_sequential, Test_run1) {
  // Create data
  std::vector<std::vector<std::pair<size_t, size_t>>> in = {{(0, 0), (1, 1)}};
  std::vector<std::vector<std::pair<size_t, size_t>>> out{{}};
  std::vector<std::vector<std::pair<size_t, size_t>>> res = {{(0, 0), (1, 1)}};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(res, out);
}

TEST(ivlev_a_convex_hull_sequential, Test_run2) {
  // Create data
  std::vector<std::vector<std::pair<size_t, size_t>>> in = {{(0, 0), (1, 1)}};
  std::vector<std::vector<std::pair<size_t, size_t>>> out{{}};
  std::vector<std::vector<std::pair<size_t, size_t>>> res = {{(0, 0), (1, 1)}};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(res, out);
}

TEST(ivlev_a_convex_hull_sequential, Test_run3) {
  // Create data
  std::vector<std::vector<std::pair<size_t, size_t>>> in = {{(0, 0), (1, 1)}};
  std::vector<std::vector<std::pair<size_t, size_t>>> out{{}};
  std::vector<std::vector<std::pair<size_t, size_t>>> res = {{(0, 0), (1, 1)}};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(res, out);
}

TEST(ivlev_a_convex_hull_sequential, Test_run5) {
  // Create data
  std::vector<std::vector<std::pair<size_t, size_t>>> in = {{(0, 0), (1, 1)}};
  std::vector<std::vector<std::pair<size_t, size_t>>> out{{}};
  std::vector<std::vector<std::pair<size_t, size_t>>> res = {{(0, 0), (1, 1)}};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(res, out);
}
