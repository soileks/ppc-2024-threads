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
  std::vector<int> outputArrayOMP(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortOmp = std::make_shared<ppc::core::TaskData>();
  hoareSortOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortOmp->inputs_count.emplace_back(inputArray.size());
  hoareSortOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayOMP.data()));
  hoareSortOmp->outputs_count.emplace_back(inputArray.size());

  HoareSortOMP HoareSortOMP(hoareSortOmp);
  ASSERT_TRUE(HoareSortOMP.validation());
  HoareSortOMP.pre_processing();
  HoareSortOMP.run();
  HoareSortOMP.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayOMP);
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
  std::vector<int> outputArrayOMP(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortOmp = std::make_shared<ppc::core::TaskData>();
  hoareSortOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortOmp->inputs_count.emplace_back(inputArray.size());
  hoareSortOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayOMP.data()));
  hoareSortOmp->outputs_count.emplace_back(inputArray.size());

  HoareSortOMP HoareSortOMP(hoareSortOmp);
  ASSERT_TRUE(HoareSortOMP.validation());
  HoareSortOMP.pre_processing();
  HoareSortOMP.run();
  HoareSortOMP.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayOMP);
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
  std::vector<int> outputArrayOMP(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortOmp = std::make_shared<ppc::core::TaskData>();
  hoareSortOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortOmp->inputs_count.emplace_back(inputArray.size());
  hoareSortOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayOMP.data()));
  hoareSortOmp->outputs_count.emplace_back(inputArray.size());

  HoareSortOMP HoareSortOMP(hoareSortOmp);
  ASSERT_TRUE(HoareSortOMP.validation());
  HoareSortOMP.pre_processing();
  HoareSortOMP.run();
  HoareSortOMP.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayOMP);
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
  std::vector<int> outputArrayOMP(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortOmp = std::make_shared<ppc::core::TaskData>();
  hoareSortOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortOmp->inputs_count.emplace_back(inputArray.size());
  hoareSortOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayOMP.data()));
  hoareSortOmp->outputs_count.emplace_back(inputArray.size());

  HoareSortOMP HoareSortOMP(hoareSortOmp);
  ASSERT_TRUE(HoareSortOMP.validation());
  HoareSortOMP.pre_processing();
  HoareSortOMP.run();
  HoareSortOMP.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayOMP);
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
  std::vector<int> outputArrayOMP(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortOmp = std::make_shared<ppc::core::TaskData>();
  hoareSortOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortOmp->inputs_count.emplace_back(inputArray.size());
  hoareSortOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayOMP.data()));
  hoareSortOmp->outputs_count.emplace_back(inputArray.size());

  HoareSortOMP HoareSortOMP(hoareSortOmp);
  ASSERT_TRUE(HoareSortOMP.validation());
  HoareSortOMP.pre_processing();
  HoareSortOMP.run();
  HoareSortOMP.post_processing();

  ASSERT_EQ(outputArraySeq, outputArrayOMP);
}