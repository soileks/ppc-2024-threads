// Copyright 2024 Vyunov Danila
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "../include/hoar_merge.h"

TEST(vyunov_d_hoare_sort_seq, Const_Array) {
  std::vector<int> inputArray = {3, 5, 6, 4, 1, 2, 0, 9, 8, 7};
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(vyunov_d_hoare_sort_seq, Random_Array_64) {
  std::vector<int> inputArray = Getvec(64);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(vyunov_d_hoare_sort_seq, Random_Array_128) {
  std::vector<int> inputArray = Getvec(128);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(vyunov_d_hoare_sort_seq, Single_Array) {
  std::vector<int> inputArray = Getvec(1);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());
  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(vyunov_d_hoare_sort_seq, Empty_Array) {
  std::vector<int> inputArray = {};
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = {};

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq HoareSortSimpleSeq(hoareSortSequential);
  ASSERT_TRUE(HoareSortSimpleSeq.validation());
  HoareSortSimpleSeq.pre_processing();
  HoareSortSimpleSeq.run();
  HoareSortSimpleSeq.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}