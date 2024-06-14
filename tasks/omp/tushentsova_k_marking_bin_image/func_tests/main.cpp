// Copyright 2024 Tushentsova Karina
#include <gtest/gtest.h>

#include <vector>

#include "omp/tushentsova_k_marking_bin_image/include/ops_omp.hpp"

TEST(tushentsova_k_marking_bin_image_omp_func, test_ones) {
  int height = 5;
  int width = 5;
  std::vector<int> size = {height, width};
  std::vector<uint8_t> in(height * width, 1);
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  MarkingInageOmp testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_omp_func, test_zeros) {
  int height = 5;
  int width = 5;
  std::vector<int> size = {height, width};
  std::vector<uint8_t> in(height * width, 1);
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  MarkingInageOmp testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_omp_func, test_all_alternation) {
  int height = 6;
  int width = 6;
  std::vector<int> size = {height, width};
  std::vector<uint8_t> in(height * width);
  std::vector<uint8_t> components(height * width);
  uint8_t count = 1;
  for (int i = 0; i < height * width; ++i) {
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

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  MarkingInageOmp testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_omp_func, test_func1) {
  int height = 5;
  int width = 5;
  std::vector<int> size = {height, width};
  std::vector<uint8_t> in = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1};
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components = {1, 1, 0, 0, 2, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 3, 0, 0, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  MarkingInageOmp testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_omp_func, test_func2) {
  int height = 3;
  int width = 6;
  std::vector<int> size = {height, width};
  std::vector<uint8_t> in = {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components = {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 2, 0, 0, 0, 1, 1};

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataSeq->inputs_count.emplace_back(size.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  MarkingInageOmp testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out, components);
}