// Copyright 2023 Kruglov Alexey
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "omp/kruglov_a_components_marking_omp/include/ops_omp.hpp"

using namespace KruglovOmpTask;

TEST(kruglov_a_img_marking_func_test, Test10_10) {
  int m = 10;
  int n = 10;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);

  std::vector<uint8_t> out_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(out_seq.data());
  taskDataSeq->outputs_count.push_back(out_seq.size());

  imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(out_seq);

  std::vector<uint8_t> out_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(out_omp.data());
  taskDataOmp->outputs_count.push_back(out_omp.size());

  imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = deserializeInt32V(out_omp);

  EXPECT_EQ(getObjectsNum(outVD_omp), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test100_100) {
  int m = 100;
  int n = 100;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);

  std::vector<uint8_t> out_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(out_seq.data());
  taskDataSeq->outputs_count.push_back(out_seq.size());

  imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(out_seq);

  std::vector<uint8_t> out_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(out_omp.data());
  taskDataOmp->outputs_count.push_back(out_omp.size());

  imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = deserializeInt32V(out_omp);

  EXPECT_EQ(getObjectsNum(outVD_omp), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test200_200) {
  int m = 100;
  int n = 100;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);

  std::vector<uint8_t> out_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(out_seq.data());
  taskDataSeq->outputs_count.push_back(out_seq.size());

  imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(out_seq);

  std::vector<uint8_t> out_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(out_omp.data());
  taskDataOmp->outputs_count.push_back(out_omp.size());

  imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = deserializeInt32V(out_omp);

  EXPECT_EQ(getObjectsNum(outVD_omp), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test300_300) {
  int m = 300;
  int n = 300;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);

  std::vector<uint8_t> out_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(out_seq.data());
  taskDataSeq->outputs_count.push_back(out_seq.size());

  imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(out_seq);

  std::vector<uint8_t> out_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(out_omp.data());
  taskDataOmp->outputs_count.push_back(out_omp.size());

  imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = deserializeInt32V(out_omp);

  EXPECT_EQ(getObjectsNum(outVD_omp), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test500_500) {
  int m = 500;
  int n = 500;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);

  std::vector<uint8_t> out_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(out_seq.data());
  taskDataSeq->outputs_count.push_back(out_seq.size());

  imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(out_seq);

  std::vector<uint8_t> out_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(out_omp.data());
  taskDataOmp->outputs_count.push_back(out_omp.size());

  imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = deserializeInt32V(out_omp);

  EXPECT_EQ(getObjectsNum(outVD_omp), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_omp, outVD_seq));
}