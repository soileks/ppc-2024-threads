// Copyright 2024 Kanakov Roman
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "seq/kanakov_r_hoare_sort/include/hoare_sort.hpp"

TEST(kanakov_r_hoare_sort_seq, Const_Array) {
  std::vector<int> inputArray = {3, 5, 6, 4, 1, 2, 0, 9, 8, 7};
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSequential HoareSortSequential(hoareSortSequential);
  ASSERT_TRUE(HoareSortSequential.validation());
  HoareSortSequential.pre_processing();
  HoareSortSequential.run();
  HoareSortSequential.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(kanakov_r_hoare_sort_seq, Random_Array) {
  std::vector<int> inputArray = generateRandomVector(64, -1000, 1000);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSequential HoareSortSequential(hoareSortSequential);
  ASSERT_TRUE(HoareSortSequential.validation());
  HoareSortSequential.pre_processing();
  HoareSortSequential.run();
  HoareSortSequential.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(kanakov_r_hoare_sort_seq, Random_Negative_Array) {
  std::vector<int> inputArray = generateRandomVector(64, -1000, -1);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSequential HoareSortSequential(hoareSortSequential);
  ASSERT_TRUE(HoareSortSequential.validation());
  HoareSortSequential.pre_processing();
  HoareSortSequential.run();
  HoareSortSequential.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(kanakov_r_hoare_sort_seq, Single_Array) {
  std::vector<int> inputArray = generateRandomVector(1, -1000, 1000);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSequential HoareSortSequential(hoareSortSequential);
  ASSERT_TRUE(HoareSortSequential.validation());
  HoareSortSequential.pre_processing();
  HoareSortSequential.run();
  HoareSortSequential.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(kanakov_r_hoare_sort_seq, Empty_Array) {
  std::vector<int> inputArray = {};
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = {};

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  HoareSortSequential HoareSortSequential(hoareSortSequential);
  ASSERT_TRUE(HoareSortSequential.validation());
  HoareSortSequential.pre_processing();
  HoareSortSequential.run();
  HoareSortSequential.post_processing();
  ASSERT_EQ(expectedArray, outputArray);
}
