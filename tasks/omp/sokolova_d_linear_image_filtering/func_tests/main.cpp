// Copyright 2024 Sokolova Daria
#include <gtest/gtest.h>

#include <vector>

#include "omp/sokolova_d_linear_image_filtering/include/ops_omp.hpp"

using namespace sokolova_omp;

TEST(sokolova_d_linear_image_filtering_omp, white_image) {
  uint32_t width = 32;
  uint32_t height = 32;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 255);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 255);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalOmp gaussFilterHorizontal(taskDataOmp);
  ASSERT_EQ(gaussFilterHorizontal.validation(), true);
  gaussFilterHorizontal.pre_processing();
  gaussFilterHorizontal.run();
  gaussFilterHorizontal.post_processing();
  ASSERT_EQ(expected, out);
}

TEST(sokolova_d_linear_image_filtering_omp, black_image) {
  uint32_t width = 32;
  uint32_t height = 32;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 0);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalOmp gaussFilterHorizontalOmp(taskDataOmp);
  ASSERT_EQ(gaussFilterHorizontalOmp.validation(), true);
  gaussFilterHorizontalOmp.pre_processing();
  gaussFilterHorizontalOmp.run();
  gaussFilterHorizontalOmp.post_processing();
  ASSERT_EQ(expected, out);
}

TEST(sokolova_d_linear_image_filtering_omp, low_resolution_image) {
  uint32_t width = 2;
  uint32_t height = 2;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 127);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 127);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalOmp gaussFilterHorizontalOmp(taskDataOmp);
  ASSERT_EQ(gaussFilterHorizontalOmp.validation(), false);
}

TEST(sokolova_d_linear_image_filtering_omp, high_resolution_image) {
  uint32_t width = 512;
  uint32_t height = 512;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 127);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 127);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalOmp gaussFilterHorizontalOmp(taskDataOmp);
  ASSERT_EQ(gaussFilterHorizontalOmp.validation(), true);
  gaussFilterHorizontalOmp.pre_processing();
  gaussFilterHorizontalOmp.run();
  gaussFilterHorizontalOmp.post_processing();
  ASSERT_EQ(expected, out);
}

TEST(sokolova_d_linear_image_filtering_omp, empty_image) {
  uint32_t width = 0;
  uint32_t height = 0;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 0);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalOmp gaussFilterHorizontalOmp(taskDataOmp);
  ASSERT_EQ(gaussFilterHorizontalOmp.validation(), false);
}
