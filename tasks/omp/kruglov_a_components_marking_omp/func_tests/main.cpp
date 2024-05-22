// Copyright 2023 Kruglov Alexey
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "omp/kruglov_a_components_marking_omp/include/ops_omp.hpp"

TEST(kruglov_a_img_marking_func_test, Test10_10) {
  int m = 10;
  int n = 10;
  auto serializedM = KruglovOmpTask::serializeInt32(m);
  auto serializedN = KruglovOmpTask::serializeInt32(n);
  std::vector<uint8_t> in = KruglovOmpTask::getRandomVector(m * n);

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

  KruglovOmpTask::imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = KruglovOmpTask::deserializeInt32V(out_seq);

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

  KruglovOmpTask::imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = KruglovOmpTask::deserializeInt32V(out_omp);

  EXPECT_EQ(KruglovOmpTask::getObjectsNum(outVD_omp), KruglovOmpTask::getObjectsNum(outVD_seq));
  EXPECT_TRUE(KruglovOmpTask::isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test100_100) {
  int m = 100;
  int n = 100;
  auto serializedM = KruglovOmpTask::serializeInt32(m);
  auto serializedN = KruglovOmpTask::serializeInt32(n);
  std::vector<uint8_t> in = KruglovOmpTask::getRandomVector(m * n);

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

  KruglovOmpTask::imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = KruglovOmpTask::deserializeInt32V(out_seq);

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

  KruglovOmpTask::imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = KruglovOmpTask::deserializeInt32V(out_omp);

  EXPECT_EQ(KruglovOmpTask::getObjectsNum(outVD_omp), KruglovOmpTask::getObjectsNum(outVD_seq));
  EXPECT_TRUE(KruglovOmpTask::isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test200_200) {
  int m = 100;
  int n = 100;
  auto serializedM = KruglovOmpTask::serializeInt32(m);
  auto serializedN = KruglovOmpTask::serializeInt32(n);
  std::vector<uint8_t> in = KruglovOmpTask::getRandomVector(m * n);

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

  KruglovOmpTask::imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = KruglovOmpTask::deserializeInt32V(out_seq);

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

  KruglovOmpTask::imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = KruglovOmpTask::deserializeInt32V(out_omp);

  EXPECT_EQ(KruglovOmpTask::getObjectsNum(outVD_omp), KruglovOmpTask::getObjectsNum(outVD_seq));
  EXPECT_TRUE(KruglovOmpTask::isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test300_300) {
  int m = 300;
  int n = 300;
  auto serializedM = KruglovOmpTask::serializeInt32(m);
  auto serializedN = KruglovOmpTask::serializeInt32(n);
  std::vector<uint8_t> in = KruglovOmpTask::getRandomVector(m * n);

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

  KruglovOmpTask::imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = KruglovOmpTask::deserializeInt32V(out_seq);

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

  KruglovOmpTask::imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = KruglovOmpTask::deserializeInt32V(out_omp);

  EXPECT_EQ(KruglovOmpTask::getObjectsNum(outVD_omp), KruglovOmpTask::getObjectsNum(outVD_seq));
  EXPECT_TRUE(KruglovOmpTask::isMapsEqual(outVD_omp, outVD_seq));
}

TEST(kruglov_a_img_marking_func_test, Test500_500) {
  int m = 500;
  int n = 500;
  auto serializedM = KruglovOmpTask::serializeInt32(m);
  auto serializedN = KruglovOmpTask::serializeInt32(n);
  std::vector<uint8_t> in = KruglovOmpTask::getRandomVector(m * n);

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

  KruglovOmpTask::imgMarkingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = KruglovOmpTask::deserializeInt32V(out_seq);

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

  KruglovOmpTask::imgMarkingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = KruglovOmpTask::deserializeInt32V(out_omp);

  EXPECT_EQ(KruglovOmpTask::getObjectsNum(outVD_omp), KruglovOmpTask::getObjectsNum(outVD_seq));
  EXPECT_TRUE(KruglovOmpTask::isMapsEqual(outVD_omp, outVD_seq));
}