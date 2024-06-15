// Copyright 2024 Lesnikov Nikita
#include <gtest/gtest.h>

#include <vector>

#include "tbb/lesnikov_nikita_binary_labelling/include/ops_tbb.hpp"

TEST(lesnikov_binary_labelling_func_test_tbb, Test10_10) {
  int m = 10;
  int n = 10;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);

  std::vector<uint8_t> outV_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(outV_seq.data());
  taskDataSeq->outputs_count.push_back(outV_seq.size());

  BinaryLabellingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(outV_seq);

  std::vector<uint8_t> outV_Tbb(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataTbb->inputs.push_back(in.data());
  taskDataTbb->inputs.push_back(serializedM.data());
  taskDataTbb->inputs.push_back(serializedN.data());
  taskDataTbb->inputs_count.push_back(in.size());
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->outputs.push_back(outV_Tbb.data());
  taskDataTbb->outputs_count.push_back(outV_Tbb.size());

  BinaryLabellingTbb testTaskTbb(taskDataTbb);
  ASSERT_TRUE(testTaskTbb.validation());
  ASSERT_TRUE(testTaskTbb.pre_processing());
  ASSERT_TRUE(testTaskTbb.run());
  ASSERT_TRUE(testTaskTbb.post_processing());

  std::vector<int> outVD_Tbb = deserializeInt32V(outV_Tbb);

  EXPECT_EQ(getObjectsNum(outVD_Tbb), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Tbb, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test_tbb, Test100_100) {
  int m = 100;
  int n = 100;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);

  std::vector<uint8_t> outV_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(outV_seq.data());
  taskDataSeq->outputs_count.push_back(outV_seq.size());

  BinaryLabellingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(outV_seq);

  std::vector<uint8_t> outV_Tbb(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataTbb->inputs.push_back(in.data());
  taskDataTbb->inputs.push_back(serializedM.data());
  taskDataTbb->inputs.push_back(serializedN.data());
  taskDataTbb->inputs_count.push_back(in.size());
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->outputs.push_back(outV_Tbb.data());
  taskDataTbb->outputs_count.push_back(outV_Tbb.size());

  BinaryLabellingTbb testTaskTbb(taskDataTbb);
  ASSERT_TRUE(testTaskTbb.validation());
  ASSERT_TRUE(testTaskTbb.pre_processing());
  ASSERT_TRUE(testTaskTbb.run());
  ASSERT_TRUE(testTaskTbb.post_processing());

  std::vector<int> outVD_Tbb = deserializeInt32V(outV_Tbb);

  EXPECT_EQ(getObjectsNum(outVD_Tbb), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Tbb, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test_tbb, Test200_200) {
  int m = 200;
  int n = 200;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);

  std::vector<uint8_t> outV_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(outV_seq.data());
  taskDataSeq->outputs_count.push_back(outV_seq.size());

  BinaryLabellingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(outV_seq);

  std::vector<uint8_t> outV_Tbb(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataTbb->inputs.push_back(in.data());
  taskDataTbb->inputs.push_back(serializedM.data());
  taskDataTbb->inputs.push_back(serializedN.data());
  taskDataTbb->inputs_count.push_back(in.size());
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->outputs.push_back(outV_Tbb.data());
  taskDataTbb->outputs_count.push_back(outV_Tbb.size());

  BinaryLabellingTbb testTaskTbb(taskDataTbb);
  ASSERT_TRUE(testTaskTbb.validation());
  ASSERT_TRUE(testTaskTbb.pre_processing());
  ASSERT_TRUE(testTaskTbb.run());
  ASSERT_TRUE(testTaskTbb.post_processing());

  std::vector<int> outVD_Tbb = deserializeInt32V(outV_Tbb);

  EXPECT_EQ(getObjectsNum(outVD_Tbb), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Tbb, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test_tbb, Test300_300) {
  int m = 300;
  int n = 300;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);

  std::vector<uint8_t> outV_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(outV_seq.data());
  taskDataSeq->outputs_count.push_back(outV_seq.size());

  BinaryLabellingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(outV_seq);

  std::vector<uint8_t> outV_Tbb(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataTbb->inputs.push_back(in.data());
  taskDataTbb->inputs.push_back(serializedM.data());
  taskDataTbb->inputs.push_back(serializedN.data());
  taskDataTbb->inputs_count.push_back(in.size());
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->outputs.push_back(outV_Tbb.data());
  taskDataTbb->outputs_count.push_back(outV_Tbb.size());

  BinaryLabellingTbb testTaskTbb(taskDataTbb);
  ASSERT_TRUE(testTaskTbb.validation());
  ASSERT_TRUE(testTaskTbb.pre_processing());
  ASSERT_TRUE(testTaskTbb.run());
  ASSERT_TRUE(testTaskTbb.post_processing());

  std::vector<int> outVD_Tbb = deserializeInt32V(outV_Tbb);

  EXPECT_EQ(getObjectsNum(outVD_Tbb), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Tbb, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test_tbb, Test400_400) {
  int m = 400;
  int n = 400;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);

  std::vector<uint8_t> outV_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(outV_seq.data());
  taskDataSeq->outputs_count.push_back(outV_seq.size());

  BinaryLabellingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(outV_seq);

  std::vector<uint8_t> outV_Tbb(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataTbb->inputs.push_back(in.data());
  taskDataTbb->inputs.push_back(serializedM.data());
  taskDataTbb->inputs.push_back(serializedN.data());
  taskDataTbb->inputs_count.push_back(in.size());
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->outputs.push_back(outV_Tbb.data());
  taskDataTbb->outputs_count.push_back(outV_Tbb.size());

  BinaryLabellingTbb testTaskTbb(taskDataTbb);
  ASSERT_TRUE(testTaskTbb.validation());
  ASSERT_TRUE(testTaskTbb.pre_processing());
  ASSERT_TRUE(testTaskTbb.run());
  ASSERT_TRUE(testTaskTbb.post_processing());

  std::vector<int> outVD_Tbb = deserializeInt32V(outV_Tbb);

  EXPECT_EQ(getObjectsNum(outVD_Tbb), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Tbb, outVD_seq));
}
