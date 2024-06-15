// Copyright 2024 Salaev Vladislav
#include <gtest/gtest.h>

#include "omp/salaev_v_components_marking_omp/include/ops_seq.hpp"

using namespace SalaevOMP;

TEST(salaev_v_components_marking_omp_functional, correct_input_data) {
  // Create data
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> input = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(input.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);

  // Create data
  std::vector<uint32_t> dimensions2 = {height, width};
  std::vector<uint8_t> input2 = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output2(height * width, 0);
  std::vector<uint32_t> expected2 = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions2.data()));
  taskDataParallel->inputs_count.emplace_back(dimensions2.size());
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(input2.data()));
  taskDataParallel->inputs_count.emplace_back(input2.size());
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(output2.data()));
  taskDataParallel->outputs_count.emplace_back(output2.size());

  // Create Task
  ImageMarkingOmp testTaskParallel(taskDataParallel);
  ASSERT_EQ(testTaskParallel.validation(), true);
}

TEST(salaev_v_components_marking_omp_functional, correct_pre_processing) {
  // Create data
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> input = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(input.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarkingSeq testTaskSequential(taskDataSeq);
  testTaskSequential.validation();
  ASSERT_EQ(testTaskSequential.pre_processing(), true);

  // Create data
  std::vector<uint32_t> dimensions2 = {height, width};
  std::vector<uint8_t> input2 = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output2(height * width, 0);
  std::vector<uint32_t> expected2 = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions2.data()));
  taskDataParallel->inputs_count.emplace_back(dimensions2.size());
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(input2.data()));
  taskDataParallel->inputs_count.emplace_back(input2.size());
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(output2.data()));
  taskDataParallel->outputs_count.emplace_back(output2.size());

  // Create Task
  ImageMarkingOmp testTaskParallel(taskDataParallel);
  testTaskParallel.validation();
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
}

TEST(salaev_v_components_marking_omp_functional, correct_run) {
  // Create data
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> input = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(input.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarkingSeq testTaskSequential(taskDataSeq);
  testTaskSequential.validation();
  testTaskSequential.pre_processing();
  ASSERT_EQ(testTaskSequential.run(), true);

  // Create data
  std::vector<uint32_t> dimensions2 = {height, width};
  std::vector<uint8_t> input2 = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output2(height * width, 0);
  std::vector<uint32_t> expected2 = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions2.data()));
  taskDataParallel->inputs_count.emplace_back(dimensions2.size());
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(input2.data()));
  taskDataParallel->inputs_count.emplace_back(input2.size());
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(output2.data()));
  taskDataParallel->outputs_count.emplace_back(output2.size());

  // Create Task
  ImageMarkingOmp testTaskParallel(taskDataParallel);
  testTaskParallel.validation();
  testTaskParallel.pre_processing();
  ASSERT_EQ(testTaskParallel.run(), true);
}

TEST(salaev_v_components_marking_omp_functional, correct_rectangle) {
  // Create data
  uint32_t height = 5;
  uint32_t width = 6;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> input = {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0,
                                1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {1, 0, 2, 0, 3,  0, 0,  4, 0,  5,  0, 6,  7, 0,  8,
                                    0, 9, 0, 0, 10, 0, 11, 0, 12, 13, 0, 14, 0, 15, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(input.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarkingSeq testTaskSequential(taskDataSeq);
  testTaskSequential.validation();
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(output, expected);

  // Create data
  std::vector<uint32_t> dimensions2 = {height, width};
  std::vector<uint8_t> input2 = {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0,
                                 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1};
  std::vector<uint32_t> output2(height * width, 0);
  std::vector<uint32_t> expected2 = {1, 0, 2, 0, 3,  0, 0,  4, 0,  5,  0, 6,  7, 0,  8,
                                     0, 9, 0, 0, 10, 0, 11, 0, 12, 13, 0, 14, 0, 15, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions2.data()));
  taskDataParallel->inputs_count.emplace_back(dimensions2.size());
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(input2.data()));
  taskDataParallel->inputs_count.emplace_back(input2.size());
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(output2.data()));
  taskDataParallel->outputs_count.emplace_back(output2.size());

  // Create Task
  ImageMarkingOmp testTaskParallel(taskDataParallel);
  testTaskParallel.validation();
  testTaskParallel.pre_processing();
  testTaskParallel.run();
  testTaskParallel.post_processing();
  ASSERT_EQ(output2, expected2);
}

TEST(salaev_v_components_marking_omp_functional, test_functional) {
  // Create data
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> input = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0, 13};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(input.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarkingSeq testTaskSequential(taskDataSeq);
  testTaskSequential.validation();
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(output, expected);

  // Create data
  std::vector<uint32_t> dimensions2 = {height, width};
  std::vector<uint8_t> input2 = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output2(height * width, 0);
  std::vector<uint32_t> expected2 = {1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0, 13};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions2.data()));
  taskDataParallel->inputs_count.emplace_back(dimensions2.size());
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(input2.data()));
  taskDataParallel->inputs_count.emplace_back(input2.size());
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(output2.data()));
  taskDataParallel->outputs_count.emplace_back(output2.size());

  // Create Task
  ImageMarkingOmp testTaskParallel(taskDataParallel);
  testTaskParallel.validation();
  testTaskParallel.pre_processing();
  testTaskParallel.run();
  testTaskParallel.post_processing();
  ASSERT_EQ(output2, expected2);
}
