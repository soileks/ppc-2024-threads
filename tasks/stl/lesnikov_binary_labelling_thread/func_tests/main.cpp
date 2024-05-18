// Copyright 2024 Lesnikov Nikita
#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "stl/lesnikov_binary_labelling_thread/include/ops_stl.hpp"

TEST(lesnikov_binary_labelling_func_test, Test10_10) {
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

  std::vector<uint8_t> outV_Thread(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataThread = std::make_shared<ppc::core::TaskData>();
  taskDataThread->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataThread->inputs.push_back(in.data());
  taskDataThread->inputs.push_back(serializedM.data());
  taskDataThread->inputs.push_back(serializedN.data());
  taskDataThread->inputs_count.push_back(in.size());
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->outputs.push_back(outV_Thread.data());
  taskDataThread->outputs_count.push_back(outV_Thread.size());

  BinaryLabellingThread testTaskThread(taskDataThread);
  ASSERT_TRUE(testTaskThread.validation());
  ASSERT_TRUE(testTaskThread.pre_processing());
  ASSERT_TRUE(testTaskThread.run());
  ASSERT_TRUE(testTaskThread.post_processing());

  std::vector<int> outVD_Thread = deserializeInt32V(outV_Thread);

  EXPECT_EQ(getObjectsNum(outVD_Thread), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Thread, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test, Test100_100) {
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

  std::vector<uint8_t> outV_Thread(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataThread = std::make_shared<ppc::core::TaskData>();
  taskDataThread->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataThread->inputs.push_back(in.data());
  taskDataThread->inputs.push_back(serializedM.data());
  taskDataThread->inputs.push_back(serializedN.data());
  taskDataThread->inputs_count.push_back(in.size());
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->outputs.push_back(outV_Thread.data());
  taskDataThread->outputs_count.push_back(outV_Thread.size());

  BinaryLabellingThread testTaskThread(taskDataThread);
  ASSERT_TRUE(testTaskThread.validation());
  ASSERT_TRUE(testTaskThread.pre_processing());
  ASSERT_TRUE(testTaskThread.run());
  ASSERT_TRUE(testTaskThread.post_processing());

  std::vector<int> outVD_Thread = deserializeInt32V(outV_Thread);

  EXPECT_EQ(getObjectsNum(outVD_Thread), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Thread, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test, Test200_200) {
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

  std::vector<uint8_t> outV_Thread(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataThread = std::make_shared<ppc::core::TaskData>();
  taskDataThread->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataThread->inputs.push_back(in.data());
  taskDataThread->inputs.push_back(serializedM.data());
  taskDataThread->inputs.push_back(serializedN.data());
  taskDataThread->inputs_count.push_back(in.size());
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->outputs.push_back(outV_Thread.data());
  taskDataThread->outputs_count.push_back(outV_Thread.size());

  BinaryLabellingThread testTaskThread(taskDataThread);
  ASSERT_TRUE(testTaskThread.validation());
  ASSERT_TRUE(testTaskThread.pre_processing());
  ASSERT_TRUE(testTaskThread.run());
  ASSERT_TRUE(testTaskThread.post_processing());

  std::vector<int> outVD_Thread = deserializeInt32V(outV_Thread);

  EXPECT_EQ(getObjectsNum(outVD_Thread), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Thread, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test, Test300_300) {
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

  std::vector<uint8_t> outV_Thread(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataThread = std::make_shared<ppc::core::TaskData>();
  taskDataThread->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataThread->inputs.push_back(in.data());
  taskDataThread->inputs.push_back(serializedM.data());
  taskDataThread->inputs.push_back(serializedN.data());
  taskDataThread->inputs_count.push_back(in.size());
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->outputs.push_back(outV_Thread.data());
  taskDataThread->outputs_count.push_back(outV_Thread.size());

  BinaryLabellingThread testTaskThread(taskDataThread);
  ASSERT_TRUE(testTaskThread.validation());
  ASSERT_TRUE(testTaskThread.pre_processing());
  ASSERT_TRUE(testTaskThread.run());
  ASSERT_TRUE(testTaskThread.post_processing());

  std::vector<int> outVD_Thread = deserializeInt32V(outV_Thread);

  EXPECT_EQ(getObjectsNum(outVD_Thread), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Thread, outVD_seq));
}

TEST(lesnikov_binary_labelling_func_test, Test400_400) {
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

  std::vector<uint8_t> outV_Thread(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataThread = std::make_shared<ppc::core::TaskData>();
  taskDataThread->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataThread->inputs.push_back(in.data());
  taskDataThread->inputs.push_back(serializedM.data());
  taskDataThread->inputs.push_back(serializedN.data());
  taskDataThread->inputs_count.push_back(in.size());
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->outputs.push_back(outV_Thread.data());
  taskDataThread->outputs_count.push_back(outV_Thread.size());

  BinaryLabellingThread testTaskThread(taskDataThread);
  ASSERT_TRUE(testTaskThread.validation());
  ASSERT_TRUE(testTaskThread.pre_processing());
  ASSERT_TRUE(testTaskThread.run());
  ASSERT_TRUE(testTaskThread.post_processing());

  std::vector<int> outVD_Thread = deserializeInt32V(outV_Thread);

  EXPECT_EQ(getObjectsNum(outVD_Thread), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_Thread, outVD_seq));
}
