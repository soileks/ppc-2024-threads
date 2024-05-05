// Copyright 2024 Khodyrev Fedor
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <stack>
#include <vector>

#include "tbb/khodyrev_f_convex_hull/include/ops_tbb.hpp"

using namespace khodyrev_tbb;

TEST(khodyrev_f_convex_hull_tbb, Test_convex_hull_validation) {
  int height = 4;
  int width = 3;
  std::vector<int> in = {0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0};
  std::vector<int> out(height * width);

  std::vector<int> true_result = {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(height + 1);
  taskDataTbb->outputs_count.emplace_back(width + 1);

  KhodyrevTaskTBB testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), false);
}

TEST(khodyrev_f_convex_hull_tbb, Test_convex_hull_with_small_image) {
  int height_in = 4;
  int width_in = 3;
  int height_out = 4;
  int width_out = 3;
  std::vector<int> in = {0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0};
  std::vector<int> out(height_out * width_out, 0);

  std::vector<int> true_result = {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();

  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(height_in);
  taskDataTbb->inputs_count.emplace_back(width_in);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(height_out);
  taskDataTbb->outputs_count.emplace_back(width_out);

  KhodyrevTaskTBB testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  ASSERT_EQ(testTaskTbb.pre_processing(), true);
  ASSERT_EQ(testTaskTbb.run(), true);
  ASSERT_EQ(testTaskTbb.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    ASSERT_EQ(out[i], true_result[i]);
  }
}

TEST(khodyrev_f_convex_hull_tbb, Test_convex_hull_with_medium_image) {
  int height_in = 1000;
  int width_in = 100;
  int height_out = 1000;
  int width_out = 100;
  std::vector<int> in(100000, 0);
  in[0] = 1;
  in[99999] = 1;
  std::vector<int> out(height_out * width_out, 0);

  std::vector<int> true_result(100000, 0);
  true_result[0] = 1;
  true_result[99999] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();

  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(height_in);
  taskDataTbb->inputs_count.emplace_back(width_in);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(height_out);
  taskDataTbb->outputs_count.emplace_back(width_out);

  KhodyrevTaskTBB testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  ASSERT_EQ(testTaskTbb.pre_processing(), true);
  ASSERT_EQ(testTaskTbb.run(), true);
  ASSERT_EQ(testTaskTbb.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    ASSERT_EQ(out[i], true_result[i]);
  }
}

TEST(khodyrev_f_convex_hull_tbb, Test_convex_hull_with_big_image) {
  int height_in = 1000;
  int width_in = 1000;
  int height_out = 1000;
  int width_out = 1000;
  std::vector<int> in(1000000, 0);
  in[0] = 1;
  in[999999] = 1;
  std::vector<int> out(height_out * width_out, 0);

  std::vector<int> true_result(1000000, 0);
  true_result[0] = 1;
  true_result[999999] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();

  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(height_in);
  taskDataTbb->inputs_count.emplace_back(width_in);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(height_out);
  taskDataTbb->outputs_count.emplace_back(width_out);

  KhodyrevTaskTBB testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  ASSERT_EQ(testTaskTbb.pre_processing(), true);
  ASSERT_EQ(testTaskTbb.run(), true);
  ASSERT_EQ(testTaskTbb.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    ASSERT_EQ(out[i], true_result[i]);
  }
}

TEST(khodyrev_f_convex_hull_tbb, Test_convex_hull_with_very_big_image) {
  int height_in = 3000;
  int width_in = 1000;
  int height_out = 3000;
  int width_out = 1000;
  std::vector<int> in(3000000, 0);
  in[0] = 1;
  in[1] = 1;
  std::vector<int> out(height_out * width_out, 0);

  std::vector<int> true_result(3000000, 0);
  true_result[0] = 1;
  true_result[1] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();

  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(height_in);
  taskDataTbb->inputs_count.emplace_back(width_in);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(height_out);
  taskDataTbb->outputs_count.emplace_back(width_out);

  KhodyrevTaskTBB testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  ASSERT_EQ(testTaskTbb.pre_processing(), true);
  ASSERT_EQ(testTaskTbb.run(), true);
  ASSERT_EQ(testTaskTbb.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    ASSERT_EQ(out[i], true_result[i]);
  }
}