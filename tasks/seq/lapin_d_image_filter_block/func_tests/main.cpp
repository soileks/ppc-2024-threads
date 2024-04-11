// Copyright 2024 Lapin Dmitriy
#include <gtest/gtest.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#include "seq/lapin_d_image_filter_block/include/ops_seq.hpp"

TEST(lapin_d_image_filter_block, Test_Lapin_test_6x8) {
  // Create data
  int height = 6;
  int width = 8;
  std::vector<std::vector<int>> in(height);
  for (int i = 0; i < height; i++) {
     in[i].resize(width);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        in[i][j] = rand() % 256;
    }
  }
  
  std::vector<std::vector<int>> out(height);
  for (int i = 0; i < height; i++) {
     out[i].resize(width);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));

  // Create Task
  BlockFilterSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
}


TEST(lapin_d_image_filter_block, Test_Lapin_test_12x30) {
  // Create data
  int height = 12;
  int width = 30;
  std::vector<std::vector<int>> in(height);
  for (int i = 0; i < height; i++) {
     in[i].resize(width);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        in[i][j] = rand() % 256;
    }
  }
  
  std::vector<std::vector<int>> out(height);
  for (int i = 0; i < height; i++) {
     out[i].resize(width);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));

  // Create Task
  BlockFilterSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
}

TEST(lapin_d_image_filter_block, Test_Lapin_test_30x90) {
  // Create data
  int height = 30;
  int width = 90;
  std::vector<std::vector<int>> in(height);
  for (int i = 0; i < height; i++) {
     in[i].resize(width);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        in[i][j] = rand() % 256;
    }
  }
  
  std::vector<std::vector<int>> out(height);
  for (int i = 0; i < height; i++) {
     out[i].resize(width);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));

  // Create Task
  BlockFilterSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
}

TEST(lapin_d_image_filter_block, Test_Lapin_test_40x50) {
  // Create data
  int height = 40;
  int width = 50;
  std::vector<std::vector<int>> in(height);
  for (int i = 0; i < height; i++) {
     in[i].resize(width);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        in[i][j] = rand() % 256;
    }
  }
  
  std::vector<std::vector<int>> out(height);
  for (int i = 0; i < height; i++) {
     out[i].resize(width);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));

  // Create Task
  BlockFilterSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
}

TEST(lapin_d_image_filter_block, Test_Lapin_test_60x70) {
  // Create data
  int height = 60;
  int width = 70;
  std::vector<std::vector<int>> in(height);
  for (int i = 0; i < height; i++) {
     in[i].resize(width);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        in[i][j] = rand() % 256;
    }
  }
  
  std::vector<std::vector<int>> out(height);
  for (int i = 0; i < height; i++) {
     out[i].resize(width);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));

  // Create Task
  BlockFilterSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
}