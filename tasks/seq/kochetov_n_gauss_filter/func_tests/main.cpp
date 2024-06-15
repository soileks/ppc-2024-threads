// Copyright 2024 Kochetov Nikolay
#include <gtest/gtest.h>

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "seq/kochetov_n_gauss_filter/include/gauss_filter.hpp"

TEST(kochetov_n_gauss_filter_seq, Test_GaussFilter_SmallImage) {
  int image[9] = {255, 255, 255, 255, 255, 255, 255, 255, 255};

  int output[9] = {255, 255, 255, 255, 255, 255, 255, 255, 255};

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(&image));
  taskData->inputs_count.emplace_back(3);
  taskData->inputs_count.emplace_back(3);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(&output));
  taskData->outputs_count.emplace_back(3);
  taskData->outputs_count.emplace_back(3);

  int result[9] = {255, 255, 255, 255, 255, 255, 255, 255, 255};

  GaussFilter gaussFilter(taskData);
  ASSERT_TRUE(gaussFilter.validation());
  gaussFilter.pre_processing();
  gaussFilter.run();
  gaussFilter.post_processing();
  for (int i = 0; i < 9; ++i) {
    EXPECT_EQ(output[i], result[i]);
  }
}

TEST(kochetov_n_gauss_filter_seq, Test_GaussFilter_SmallImage2) {
  int image[9] = {1, 50, 3, 4, 4, 0, 7, 8, 98};

  int output[9] = {1, 50, 3, 4, 4, 0, 7, 8, 98};

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(&image));
  taskData->inputs_count.emplace_back(3);
  taskData->inputs_count.emplace_back(3);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(&output));
  taskData->outputs_count.emplace_back(3);
  taskData->outputs_count.emplace_back(3);

  int result[9] = {1, 50, 3, 4, 15, 0, 7, 8, 98};

  GaussFilter gaussFilter(taskData);
  ASSERT_TRUE(gaussFilter.validation());
  gaussFilter.pre_processing();
  gaussFilter.run();
  gaussFilter.post_processing();
  for (int i = 0; i < 9; ++i) {
    EXPECT_EQ(output[i], result[i]);
  }
}

TEST(kochetov_n_gauss_filter_seq, Test_GaussFilter_EmptyImage) {
  int* image = {};
  int* output = {};

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(&image));
  taskData->inputs_count.emplace_back(0);
  taskData->inputs_count.emplace_back(0);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(&output));
  taskData->outputs_count.emplace_back(0);
  taskData->outputs_count.emplace_back(0);

  GaussFilter gaussFilter(taskData);
  ASSERT_FALSE(gaussFilter.validation());
}

TEST(kochetov_n_gauss_filter_seq, Test_GaussFilter_SinglePixel) {
  int image[1] = {128};
  int output[1] = {128};

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(&image));
  taskData->inputs_count.emplace_back(1);
  taskData->inputs_count.emplace_back(1);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(&output));
  taskData->outputs_count.emplace_back(1);
  taskData->outputs_count.emplace_back(1);

  int result[1] = {128};

  GaussFilter gaussFilter(taskData);
  ASSERT_FALSE(gaussFilter.validation());
  EXPECT_EQ(output[0], result[0]);
}

TEST(kochetov_n_gauss_filter_seq, Test_GaussFilter_LargeImage) {
  int image[36] = {0,  2,  4,  6,  8,  10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34,
                   36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70};

  int output[36] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(&image));
  taskData->inputs_count.emplace_back(6);
  taskData->inputs_count.emplace_back(6);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(&output));
  taskData->outputs_count.emplace_back(6);
  taskData->outputs_count.emplace_back(6);

  int result[36] = {0, 0,  0,  0,  0,  0, 0, 14, 16, 18, 20, 0, 0, 26, 28, 30, 32, 0,
                    0, 38, 40, 42, 44, 0, 0, 50, 52, 54, 56, 0, 0, 0,  0,  0,  0,  0};

  GaussFilter gaussFilter(taskData);
  ASSERT_TRUE(gaussFilter.validation());
  gaussFilter.pre_processing();
  gaussFilter.run();
  gaussFilter.post_processing();
  for (int i = 0; i < 36; ++i) {
    EXPECT_EQ(output[i], result[i]);
  }
}

TEST(kochetov_n_gauss_filter_seq, Test_GaussFilter_BorderEffect) {
  int image[9] = {255, 255, 255, 255, 0, 255, 255, 255, 255};
  int output[9] = {255, 255, 255, 255, 0, 255, 255, 255, 255};

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(&image));
  taskData->inputs_count.emplace_back(3);
  taskData->inputs_count.emplace_back(3);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(&output));
  taskData->outputs_count.emplace_back(3);
  taskData->outputs_count.emplace_back(3);

  int result[9] = {255, 255, 255, 255, 191, 255, 255, 255, 255};

  GaussFilter gaussFilter(taskData);
  ASSERT_TRUE(gaussFilter.validation());
  gaussFilter.pre_processing();
  gaussFilter.run();
  gaussFilter.post_processing();
  for (int i = 0; i < 9; ++i) {
    EXPECT_EQ(output[i], result[i]);
  }
}
