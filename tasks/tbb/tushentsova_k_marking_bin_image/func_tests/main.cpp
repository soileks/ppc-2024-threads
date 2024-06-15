// Copyright 2024 Tushentsova Karina

#include <gtest/gtest.h>

#include <vector>

#include "tbb/tushentsova_k_marking_bin_image/include/ops_tbb.hpp"

TEST(tushentsova_k_marking_bin_image_tbb_func, test_ones) {
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> size = {height, width};
  std::vector<uint8_t> in(height * width, 1);
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataPar->inputs_count.emplace_back(size.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  markingImageTBB testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  testTaskParallel.pre_processing();
  testTaskParallel.run();
  testTaskParallel.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_tbb_func, test_zeros) {
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> size = {height, width};
  std::vector<uint8_t> in(height * width, 1);
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataPar->inputs_count.emplace_back(size.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  markingImageTBB testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  testTaskParallel.pre_processing();
  testTaskParallel.run();
  testTaskParallel.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_tbb_func, test_all_alternation) {
  uint32_t height = 6;
  uint32_t width = 6;
  std::vector<uint32_t> size = {height, width};
  std::vector<uint8_t> in(height * width);
  std::vector<uint8_t> components(height * width);
  uint8_t count = 1;
  for (uint32_t i = 0; i < height * width; ++i) {
    if (i % 2 == 0) {
      in[i] = 0;
      components[i] = count++;
      if (count > width / 2) count = 1;
    } else {
      in[i] = 1;
      components[i] = 0;
    }
  }
  std::vector<uint8_t> out(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataPar->inputs_count.emplace_back(size.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  markingImageTBB testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  testTaskParallel.pre_processing();
  testTaskParallel.run();
  testTaskParallel.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_tbb_func, test_func1) {
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> size = {height, width};
  std::vector<uint8_t> in = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1};
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components = {1, 1, 0, 0, 2, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 3, 0, 0, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataPar->inputs_count.emplace_back(size.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  markingImageTBB testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  testTaskParallel.pre_processing();
  testTaskParallel.run();
  testTaskParallel.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_tbb_func, test_func2) {
  uint32_t height = 3;
  uint32_t width = 6;
  std::vector<uint32_t> size = {height, width};
  std::vector<uint8_t> in = {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components = {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 2, 0, 0, 0, 1, 1};

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataPar->inputs_count.emplace_back(size.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  markingImageTBB testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  testTaskParallel.pre_processing();
  testTaskParallel.run();
  testTaskParallel.post_processing();
  ASSERT_EQ(out, components);
}