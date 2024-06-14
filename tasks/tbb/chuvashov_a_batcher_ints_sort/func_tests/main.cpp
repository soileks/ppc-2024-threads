// Copyright 2024 Chuvashov Andrey
#include <gtest/gtest.h>

#include <vector>

#include "tbb/chuvashov_a_batcher_ints_sort/include/batcher_ints_sort.hpp"

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_Simple_Array) {
  std::vector<int> vect = {774, -434, 531, 615, -617, -548, 618, -924, -162, 577, 797, -214, -851, -723, 983, -910};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_100_Size) {
  std::vector<int> vect = ChuvashovTBB_GenerateVector(100);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_200_Size) {
  std::vector<int> vect = ChuvashovTBB_GenerateVector(200);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_500_Size) {
  std::vector<int> vect = ChuvashovTBB_GenerateVector(500);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_1000_Size) {
  std::vector<int> vect = ChuvashovTBB_GenerateVector(1000);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_Little_Size) {
  std::vector<int> vect = ChuvashovTBB_GenerateVector(4);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_Only_Negative) {
  std::vector<int> vect = {-90, -429, -130, -203, -1090, -102, -34, -1, -56, -43, -190, -45};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_Only_Zeroes) {
  std::vector<int> vect = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsTBBs, TBB_Test_Already_Sorted) {
  std::vector<int> vect = {100, 225, 314, 419, 512, 628, 711, 839, 943, 1130};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  Chuvashov_TBBBatcherSort testTbb(tbbData);
  ASSERT_EQ(testTbb.validation(), true);
  testTbb.pre_processing();
  testTbb.run();
  testTbb.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}
