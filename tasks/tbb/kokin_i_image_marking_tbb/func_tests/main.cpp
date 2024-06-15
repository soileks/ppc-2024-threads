// Copyright 2024 Kokin Ivan
#include <gtest/gtest.h>

#include <vector>

#include "tbb/kokin_i_image_marking_tbb/include/ops_tbb.hpp"

TEST(kokin_marking_component_bin_image_tbb, test_5_5) {
  uint32_t ht = 5;
  uint32_t wh = 5;
  std::vector<uint32_t> size = {ht, wh};
  std::vector<uint8_t> in(ht * wh, 1);
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt(ht * wh, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataTBB->inputs_count.emplace_back(size.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  imageMarkingTBB testTaskSequential(taskDataTBB);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image_tbb, test_10_10) {
  uint32_t ht = 10;
  uint32_t wh = 10;
  std::vector<uint32_t> size = {ht, wh};
  std::vector<uint8_t> in(ht * wh, 0);
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt(ht * wh, 1);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataTBB->inputs_count.emplace_back(size.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  imageMarkingTBB testTaskSequential(taskDataTBB);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image_tbb, test_3_6) {
  uint32_t ht = 3;
  uint32_t wh = 6;
  std::vector<uint32_t> size = {ht, wh};
  std::vector<uint8_t> in = {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt = {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 2, 0, 0, 0, 1, 1};

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataTBB->inputs_count.emplace_back(size.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  imageMarkingTBB testTaskSequential(taskDataTBB);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image_tbb, test_4) {
  uint32_t ht = 5;
  uint32_t wh = 10;
  std::vector<uint32_t> size = {ht, wh};
  std::vector<uint8_t> in = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt = {1, 1, 0, 0, 2, 2, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataTBB->inputs_count.emplace_back(size.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  imageMarkingTBB testTaskSequential(taskDataTBB);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image_tbb, test_5_5_arr) {
  uint32_t ht = 5;
  uint32_t wh = 5;
  std::vector<uint32_t> size = {ht, wh};
  std::vector<uint8_t> in = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1};
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt = {1, 1, 0, 0, 2, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 3, 0, 0, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataTBB->inputs_count.emplace_back(size.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  imageMarkingTBB testTaskSequential(taskDataTBB);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}
