// Copyright 2024 Sokolova Daria
#include <gtest/gtest.h>

#include <vector>

#include "tbb/sokolova_d_linear_image_filtering/include/ops_tbb.hpp"

using namespace sokolova_tbb;

TEST(sokolova_d_linear_image_filtering_tbb, white_image) {
  uint32_t width = 32;
  uint32_t height = 32;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 255);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 255);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalTbb gaussFilterHorizontal(taskDataTbb);
  ASSERT_EQ(gaussFilterHorizontal.validation(), true);
  gaussFilterHorizontal.pre_processing();
  gaussFilterHorizontal.run();
  gaussFilterHorizontal.post_processing();
  ASSERT_EQ(expected, out);
}

TEST(sokolova_d_linear_image_filtering_tbb, black_image) {
  uint32_t width = 32;
  uint32_t height = 32;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 0);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalTbb gaussFilterHorizontalTbb(taskDataTbb);
  ASSERT_EQ(gaussFilterHorizontalTbb.validation(), true);
  gaussFilterHorizontalTbb.pre_processing();
  gaussFilterHorizontalTbb.run();
  gaussFilterHorizontalTbb.post_processing();
  ASSERT_EQ(expected, out);
}

TEST(sokolova_d_linear_image_filtering_tbb, low_resolution_image) {
  uint32_t width = 2;
  uint32_t height = 2;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 127);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 127);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalTbb gaussFilterHorizontalTbb(taskDataTbb);
  ASSERT_EQ(gaussFilterHorizontalTbb.validation(), false);
}

TEST(sokolova_d_linear_image_filtering_tbb, high_resolution_image) {
  uint32_t width = 512;
  uint32_t height = 512;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 127);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 127);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalTbb gaussFilterHorizontalTbb(taskDataTbb);
  ASSERT_EQ(gaussFilterHorizontalTbb.validation(), true);
  gaussFilterHorizontalTbb.pre_processing();
  gaussFilterHorizontalTbb.run();
  gaussFilterHorizontalTbb.post_processing();
  ASSERT_EQ(expected, out);
}

TEST(sokolova_d_linear_image_filtering_tbb, empty_image) {
  uint32_t width = 0;
  uint32_t height = 0;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 0);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  GaussFilterHorizontalTbb gaussFilterHorizontalTbb(taskDataTbb);
  ASSERT_EQ(gaussFilterHorizontalTbb.validation(), false);
}
