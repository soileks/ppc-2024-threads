// Copyright 2024 Vyunov Danila
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "../include/hoar_merge.h"

TEST(vyunov_d_hoare_sort_omp, Const_Array) {
  std::vector<int> inputArray = {3, 5, 6, 4, 1, 2, 0, 9, 8, 7};
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortTbb = std::make_shared<ppc::core::TaskData>();
  hoareSortTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortTbb->inputs_count.emplace_back(inputArray.size());
  hoareSortTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortTbb->outputs_count.emplace_back(inputArray.size());

  HoareSortTBBV HoareSortTBB(hoareSortTbb);
  ASSERT_TRUE(HoareSortTBB.validation());
  HoareSortTBB.pre_processing();
  HoareSortTBB.run();
  HoareSortTBB.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}

TEST(vyunov_d_hoare_sort_omp, Random_Array_64) {
  std::vector<int> inputArray = Getvec(64);
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortTbb = std::make_shared<ppc::core::TaskData>();
  hoareSortTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortTbb->inputs_count.emplace_back(inputArray.size());
  hoareSortTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortTbb->outputs_count.emplace_back(inputArray.size());

  HoareSortTBBV HoareSortTBB(hoareSortTbb);
  ASSERT_TRUE(HoareSortTBB.validation());
  HoareSortTBB.pre_processing();
  HoareSortTBB.run();
  HoareSortTBB.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}

TEST(vyunov_d_hoare_sort_omp, Random_Array_128) {
  std::vector<int> inputArray = Getvec(128);
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortTbb = std::make_shared<ppc::core::TaskData>();
  hoareSortTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortTbb->inputs_count.emplace_back(inputArray.size());
  hoareSortTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortTbb->outputs_count.emplace_back(inputArray.size());

  HoareSortTBBV HoareSortTBB(hoareSortTbb);
  ASSERT_TRUE(HoareSortTBB.validation());
  HoareSortTBB.pre_processing();
  HoareSortTBB.run();
  HoareSortTBB.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}

TEST(vyunov_d_hoare_sort_omp, Single_Array) {
  std::vector<int> inputArray = Getvec(1);
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortTbb = std::make_shared<ppc::core::TaskData>();
  hoareSortTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortTbb->inputs_count.emplace_back(inputArray.size());
  hoareSortTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortTbb->outputs_count.emplace_back(inputArray.size());

  HoareSortTBBV HoareSortTBB(hoareSortTbb);
  ASSERT_TRUE(HoareSortTBB.validation());
  HoareSortTBB.pre_processing();
  HoareSortTBB.run();
  HoareSortTBB.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}

TEST(vyunov_d_hoare_sort_omp, Empty_Array) {
  std::vector<int> inputArray = {};
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortTbb = std::make_shared<ppc::core::TaskData>();
  hoareSortTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortTbb->inputs_count.emplace_back(inputArray.size());
  hoareSortTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortTbb->outputs_count.emplace_back(inputArray.size());

  HoareSortTBBV HoareSortTBB(hoareSortTbb);
  ASSERT_TRUE(HoareSortTBB.validation());
  HoareSortTBB.pre_processing();
  HoareSortTBB.run();
  HoareSortTBB.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}