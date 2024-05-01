// Copyright 2024 Kokin Ivan
#include <gtest/gtest.h>

#include <vector>

#include "omp/kokin_marking_component_bin_image/include/ops_omp.hpp"

TEST(kokin_marking_component_bin_image, test_5_5) {
  int ht = 5;
  int wh = 5;
  std::vector<int> size = {ht, wh};
  std::vector<uint8_t> in(ht * wh, 1);
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt(ht * wh, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  imageMarkingOMP testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image, test_10_10) {
  int ht = 10;
  int wh = 10;
  std::vector<int> size = {ht, wh};
  std::vector<uint8_t> in(ht * wh, 0);
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt(ht * wh, 1);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  imageMarkingOMP testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image, test_3_6) {
  int ht = 3;
  int wh = 6;
  std::vector<int> size = {ht, wh};
  std::vector<uint8_t> in = {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt = {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 2, 0, 0, 0, 1, 1};

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  imageMarkingOMP testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image, test_4) {
  int ht = 5;
  int wh = 10;
  std::vector<int> size = {ht, wh};
  std::vector<uint8_t> in = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt = {1, 1, 0, 0, 2, 2, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  imageMarkingOMP testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image, test_5_5_arr) {
  int ht = 5;
  int wh = 5;
  std::vector<int> size = {ht, wh};;
  std::vector<uint8_t> in = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1};
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt = {1, 1, 0, 0, 2, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 3, 0, 0, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  imageMarkingOMP testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, cmpt);
}
