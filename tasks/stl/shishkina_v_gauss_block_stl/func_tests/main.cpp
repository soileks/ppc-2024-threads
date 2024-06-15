// Copyright 2023 Shishkina Valeriya
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <thread>
#include <vector>

#include "stl/shishkina_v_gauss_block_stl/include/ops_stl.hpp"

TEST(shishkina_v_gauss_block_stl, test1_stl) {
  int n = 3;
  int m = 3;
  auto count = std::thread::hardware_concurrency() * 2;
  // Create data
  std::vector<int> tmp = {255, 255, 255, 255, 255, 255, 255, 255, 255};

  int min = *std::min_element(tmp.begin(), tmp.end());
  int max = *std::max_element(tmp.begin(), tmp.end());

  std::vector<int> tmp_sec = {n, m};
  std::vector<int> tmp_third = {min, max};
  std::vector<int> out(n * m);

  std::vector<int> res = {255, 255, 255, 255, 255, 255, 255, 255, 255};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_sec.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_sec.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp.data()));
  taskDataSeq->inputs_count.emplace_back(tmp.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_third.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_third.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  LinearFilteringGauss testSTLTaskSequential(taskDataSeq);
  testSTLTaskSequential.setNumThreads(count);
  ASSERT_EQ(testSTLTaskSequential.validation(), true);
  testSTLTaskSequential.pre_processing();
  ASSERT_EQ(testSTLTaskSequential.run(), true);
  testSTLTaskSequential.post_processing();
  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(res[i], out[i]);
  }
}

TEST(shishkina_v_gauss_block_stl, test2_stl) {
  int n = 3;
  int m = 3;
  auto count = std::thread::hardware_concurrency() * 3;

  // Create data
  std::vector<int> tmp = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  int min = *std::min_element(tmp.begin(), tmp.end());
  int max = *std::max_element(tmp.begin(), tmp.end());

  std::vector<int> tmp_sec = {n, m};
  std::vector<int> tmp_third = {min, max};
  std::vector<int> out(n * m);

  std::vector<int> res = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_sec.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_sec.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp.data()));
  taskDataSeq->inputs_count.emplace_back(tmp.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_third.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_third.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  LinearFilteringGauss testSTLTaskSequential(taskDataSeq);
  testSTLTaskSequential.setNumThreads(count);
  ASSERT_EQ(testSTLTaskSequential.validation(), true);
  testSTLTaskSequential.pre_processing();
  ASSERT_EQ(testSTLTaskSequential.run(), true);
  testSTLTaskSequential.post_processing();
  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(res[i], out[i]);
  }
}

TEST(shishkina_v_gauss_block_stl, test3_stl) {
  int n = 4;
  int m = 4;
  auto count = std::thread::hardware_concurrency() * 4;
  // Create data
  std::vector<int> tmp = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0};

  int min = *std::min_element(tmp.begin(), tmp.end());
  int max = *std::max_element(tmp.begin(), tmp.end());

  std::vector<int> tmp_sec = {n, m};
  std::vector<int> tmp_third = {min, max};
  std::vector<int> out(n * m);

  std::vector<int> res = {4, 6, 6, 5, 6, 8, 8, 6, 6, 8, 8, 6, 5, 6, 6, 4};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_sec.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_sec.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp.data()));
  taskDataSeq->inputs_count.emplace_back(tmp.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_third.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_third.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  LinearFilteringGauss testSTLTaskSequential(taskDataSeq);
  testSTLTaskSequential.setNumThreads(count);
  ASSERT_EQ(testSTLTaskSequential.validation(), true);
  testSTLTaskSequential.pre_processing();
  ASSERT_EQ(testSTLTaskSequential.run(), true);
  testSTLTaskSequential.post_processing();
  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(res[i], out[i]);
  }
}

TEST(shishkina_v_gauss_block_stl, test4_stl) {
  int n = 5;
  int m = 5;
  auto count = std::thread::hardware_concurrency() * 5;

  // Create data
  std::vector<int> tmp = {0,   255, 0,   255, 0,   255, 0,   255, 0,   255, 0,   255, 0,
                          255, 0,   255, 0,   255, 0,   255, 0,   255, 0,   255, 0};

  int min = *std::min_element(tmp.begin(), tmp.end());
  int max = *std::max_element(tmp.begin(), tmp.end());

  std::vector<int> tmp_sec = {n, m};
  std::vector<int> tmp_third = {min, max};
  std::vector<int> out(n * m);

  std::vector<int> res = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                          255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_sec.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_sec.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp.data()));
  taskDataSeq->inputs_count.emplace_back(tmp.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_third.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_third.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  LinearFilteringGauss testSTLTaskSequential(taskDataSeq);
  testSTLTaskSequential.setNumThreads(count);
  ASSERT_EQ(testSTLTaskSequential.validation(), true);
  testSTLTaskSequential.pre_processing();
  ASSERT_EQ(testSTLTaskSequential.run(), true);
  testSTLTaskSequential.post_processing();
  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(res[i], out[i]);
  }
}

TEST(shishkina_v_gauss_block_stl, test5_stl) {
  int n = 3;
  int m = 5;
  auto count = std::thread::hardware_concurrency() * 6;

  // Create data
  std::vector<int> tmp = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

  int min = *std::min_element(tmp.begin(), tmp.end());
  int max = *std::max_element(tmp.begin(), tmp.end());

  std::vector<int> tmp_sec = {n, m};
  std::vector<int> tmp_third = {min, max};
  std::vector<int> out(n * m);

  std::vector<int> res = {5, 6, 6, 6, 5, 6, 8, 8, 8, 6, 5, 6, 6, 6, 5};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_sec.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_sec.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp.data()));
  taskDataSeq->inputs_count.emplace_back(tmp.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp_third.data()));
  taskDataSeq->inputs_count.emplace_back(tmp_third.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  LinearFilteringGauss testSTLTaskSequential(taskDataSeq);
  testSTLTaskSequential.setNumThreads(count);
  ASSERT_EQ(testSTLTaskSequential.validation(), true);
  testSTLTaskSequential.pre_processing();
  ASSERT_EQ(testSTLTaskSequential.run(), true);
  testSTLTaskSequential.post_processing();
  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(res[i], out[i]);
  }
}
