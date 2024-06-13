// Copyright 2024 Gontsov Roman
#include <gtest/gtest.h>

#include <vector>

#include "stl/gontsov_r_merge_sort_stl/include/ops_stl.hpp"

TEST(Gontsov_R_Radix_Sort, can_create_task_successfully) {
  std::vector<int> in = {45, 12, 89};
  std::vector<int> exp = {12, 45, 89};

  // Create data
  std::vector<int> out2(exp.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataPar->outputs_count.emplace_back(out2.size());

  // Create Task
  RadixTBBG RadixTBBG(taskDataPar);
  ASSERT_EQ(RadixTBBG.validation(), true);
  ASSERT_TRUE(RadixTBBG.pre_processing());
  ASSERT_TRUE(RadixTBBG.run());
  ASSERT_TRUE(RadixTBBG.post_processing());
  for (size_t i = 0; i < out2.size(); i++) {
    ASSERT_EQ(out2[i], exp[i]);
  }
}

TEST(Gontsov_R_Radix_Sort, test2) {
  std::vector<int> in = {45, 12, 89, 33, 27, 150, 2, 58, 9, 66};
  std::vector<int> exp = {2, 9, 12, 27, 33, 45, 58, 66, 89, 150};

  // Create data
  std::vector<int> out2(exp.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataPar->outputs_count.emplace_back(out2.size());

  // Create Task
  RadixTBBG RadixTBBG(taskDataPar);
  ASSERT_EQ(RadixTBBG.validation(), true);
  ASSERT_TRUE(RadixTBBG.pre_processing());
  ASSERT_TRUE(RadixTBBG.run());
  ASSERT_TRUE(RadixTBBG.post_processing());

  for (size_t i = 0; i < out2.size(); i++) {
    ASSERT_EQ(out2[i], exp[i]);
  }
}

TEST(Gontsov_R_Radix_Sort, test3) {
  std::vector<int> in = {45, 12};
  std::vector<int> exp = {12, 45};

  // Create data
  std::vector<int> out2(exp.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataPar->outputs_count.emplace_back(out2.size());

  // Create Task
  RadixTBBG RadixTBBG(taskDataPar);
  ASSERT_EQ(RadixTBBG.validation(), true);
  ASSERT_TRUE(RadixTBBG.pre_processing());
  ASSERT_TRUE(RadixTBBG.run());
  ASSERT_TRUE(RadixTBBG.post_processing());

  for (size_t i = 0; i < out2.size(); i++) {
    ASSERT_EQ(out2[i], exp[i]);
  }
}

TEST(Gontsov_R_Radix_Sort, test4) {
  std::vector<int> in = {45, 12, 89};
  std::vector<int> exp = {12, 45, 89};

  // Create data
  std::vector<int> out2(exp.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataPar->outputs_count.emplace_back(out2.size());

  // Create Task
  RadixTBBG RadixTBBG(taskDataPar);
  ASSERT_EQ(RadixTBBG.validation(), true);
  ASSERT_TRUE(RadixTBBG.pre_processing());
  ASSERT_TRUE(RadixTBBG.run());
  ASSERT_TRUE(RadixTBBG.post_processing());

  for (size_t i = 0; i < out2.size(); i++) {
    ASSERT_EQ(out2[i], exp[i]);
  }
}

TEST(Gontsov_R_Radix_Sort, test5) {
  std::vector<int> in = {45, 12, 89, 33, 27, -150, -2, 58, 9, 66};
  std::vector<int> exp = {-150, -2, 9, 12, 27, 33, 45, 58, 66, 89};

  // Create data
  std::vector<int> out2(exp.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataPar->outputs_count.emplace_back(out2.size());

  // Create Task
  RadixTBBG RadixTBBG(taskDataPar);
  ASSERT_EQ(RadixTBBG.validation(), true);
  ASSERT_TRUE(RadixTBBG.pre_processing());
  ASSERT_TRUE(RadixTBBG.run());
  ASSERT_TRUE(RadixTBBG.post_processing());

  for (size_t i = 0; i < out2.size(); i++) {
    ASSERT_EQ(out2[i], exp[i]);
  }
}
