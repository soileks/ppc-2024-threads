// Copyright 2024 Chuvashov Andrey
#include <gtest/gtest.h>

#include <vector>

#include "omp/chuvashov_a_batcher_ints_sort/include/batcher_ints_sort.hpp"

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_Simple_Array) {
  std::vector<int> vect = {774, -434, 531, 615, -617, -548, 618, -924, -162, 577, 797, -214, -851, -723, 983, -910};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_100_Size) {
  std::vector<int> vect = ChuvashovOMP_GenerateVector(100);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_200_Size) {
  std::vector<int> vect = ChuvashovOMP_GenerateVector(200);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_500_Size) {
  std::vector<int> vect = ChuvashovOMP_GenerateVector(500);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_1000_Size) {
  std::vector<int> vect = ChuvashovOMP_GenerateVector(1000);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_Little_Size) {
  std::vector<int> vect = ChuvashovOMP_GenerateVector(4);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_Only_Negative) {
  std::vector<int> vect = {-90, -429, -130, -203, -1090, -102, -34, -1, -56, -43, -190, -45};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_Only_Zeroes) {
  std::vector<int> vect = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherFuncTestsOMPs, OMP_Test_Already_Sorted) {
  std::vector<int> vect = {100, 225, 314, 419, 512, 628, 711, 839, 943, 1130};
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> ompData = std::make_shared<ppc::core::TaskData>();
  ompData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  ompData->inputs_count.emplace_back(vect.size());
  ompData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  ompData->outputs_count.emplace_back(result.size());

  Chuvashov_OMPBatcherSort testOmp(ompData);
  ASSERT_EQ(testOmp.validation(), true);
  testOmp.pre_processing();
  testOmp.run();
  testOmp.post_processing();
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}
