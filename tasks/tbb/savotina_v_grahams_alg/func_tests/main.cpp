// Copyright 2024 Savotina Valeria

#include <gtest/gtest.h>

// #include <iostream>
#include "tbb/savotina_v_grahams_alg/include/ops_tbb.hpp"

// Points are arranged in a chaotic order (type of x and y is int)
TEST(savotina_v_grahams_alg_tbb, Test1) {
  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> points = {
      SavotinaTbb::SavotinaPoint(-8, 4),   SavotinaTbb::SavotinaPoint(-4, 6),   SavotinaTbb::SavotinaPoint(-12, 2),
      SavotinaTbb::SavotinaPoint(-6, -2),  SavotinaTbb::SavotinaPoint(-10, -4), SavotinaTbb::SavotinaPoint(-4, 2),
      SavotinaTbb::SavotinaPoint(-6, 6),   SavotinaTbb::SavotinaPoint(-8, 8),   SavotinaTbb::SavotinaPoint(-10, 6),
      SavotinaTbb::SavotinaPoint(-8, 2),   SavotinaTbb::SavotinaPoint(-6, 2),   SavotinaTbb::SavotinaPoint(-10, 0),
      SavotinaTbb::SavotinaPoint(-14, -2), SavotinaTbb::SavotinaPoint(-16, -4), SavotinaTbb::SavotinaPoint(-14, -4)};
  std::vector<SavotinaTbb::SavotinaPoint> mchSeq(7);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> pointsTbb = {
      SavotinaTbb::SavotinaPoint(-8, 4),   SavotinaTbb::SavotinaPoint(-4, 6),   SavotinaTbb::SavotinaPoint(-12, 2),
      SavotinaTbb::SavotinaPoint(-6, -2),  SavotinaTbb::SavotinaPoint(-10, -4), SavotinaTbb::SavotinaPoint(-4, 2),
      SavotinaTbb::SavotinaPoint(-6, 6),   SavotinaTbb::SavotinaPoint(-8, 8),   SavotinaTbb::SavotinaPoint(-10, 6),
      SavotinaTbb::SavotinaPoint(-8, 2),   SavotinaTbb::SavotinaPoint(-6, 2),   SavotinaTbb::SavotinaPoint(-10, 0),
      SavotinaTbb::SavotinaPoint(-14, -2), SavotinaTbb::SavotinaPoint(-16, -4), SavotinaTbb::SavotinaPoint(-14, -4)};
  std::vector<SavotinaTbb::SavotinaPoint> mchTbb(7);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(pointsTbb.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(pointsTbb.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchTbb.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mchTbb.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmTbbParallel testGrahamsAlgTbb(dataGrahamsAlgTbb);
  ASSERT_EQ(testGrahamsAlgTbb.validation(), true);
  testGrahamsAlgTbb.pre_processing();
  testGrahamsAlgTbb.run();
  testGrahamsAlgTbb.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i].x, mchTbb[i].x);
    ASSERT_EQ(mchSeq[i].y, mchTbb[i].y);
  }
}

// Points are arranged in a chaotic order (type of x and y is double)
TEST(savotina_v_grahams_alg_tbb, Test2) {
  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> points = {
      SavotinaTbb::SavotinaPoint(-0.5, 2.2),  SavotinaTbb::SavotinaPoint(1.6, 1.3),
      SavotinaTbb::SavotinaPoint(0.3, -0.6),  SavotinaTbb::SavotinaPoint(0.1, 1.3),
      SavotinaTbb::SavotinaPoint(-1.8, 1.42), SavotinaTbb::SavotinaPoint(-0.3, 0.6),
      SavotinaTbb::SavotinaPoint(-0.5, -1.2), SavotinaTbb::SavotinaPoint(1.2, -0.8),
      SavotinaTbb::SavotinaPoint(0.7, 0.4),   SavotinaTbb::SavotinaPoint(1.1, 1.9),
      SavotinaTbb::SavotinaPoint(0.4, -1.2),  SavotinaTbb::SavotinaPoint(-1.9, 0.4),
      SavotinaTbb::SavotinaPoint(-0.6, -0.3), SavotinaTbb::SavotinaPoint(1.8, 0.5),
      SavotinaTbb::SavotinaPoint(-1.4, -0.7), SavotinaTbb::SavotinaPoint(-0.9, 1.1),
      SavotinaTbb::SavotinaPoint(-1.2, 1.9),  SavotinaTbb::SavotinaPoint(0.4, 2.2),
      SavotinaTbb::SavotinaPoint(1.7, -0.1)};
  std::vector<SavotinaTbb::SavotinaPoint> mchSeq(13);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> pointsTbb = {
      SavotinaTbb::SavotinaPoint(-0.5, 2.2),  SavotinaTbb::SavotinaPoint(1.6, 1.3),
      SavotinaTbb::SavotinaPoint(0.3, -0.6),  SavotinaTbb::SavotinaPoint(0.1, 1.3),
      SavotinaTbb::SavotinaPoint(-1.8, 1.42), SavotinaTbb::SavotinaPoint(-0.3, 0.6),
      SavotinaTbb::SavotinaPoint(-0.5, -1.2), SavotinaTbb::SavotinaPoint(1.2, -0.8),
      SavotinaTbb::SavotinaPoint(0.7, 0.4),   SavotinaTbb::SavotinaPoint(1.1, 1.9),
      SavotinaTbb::SavotinaPoint(0.4, -1.2),  SavotinaTbb::SavotinaPoint(-1.9, 0.4),
      SavotinaTbb::SavotinaPoint(-0.6, -0.3), SavotinaTbb::SavotinaPoint(1.8, 0.5),
      SavotinaTbb::SavotinaPoint(-1.4, -0.7), SavotinaTbb::SavotinaPoint(-0.9, 1.1),
      SavotinaTbb::SavotinaPoint(-1.2, 1.9),  SavotinaTbb::SavotinaPoint(0.4, 2.2),
      SavotinaTbb::SavotinaPoint(1.7, -0.1)};
  std::vector<SavotinaTbb::SavotinaPoint> mchTbb(13);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(pointsTbb.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(pointsTbb.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchTbb.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mchTbb.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmTbbParallel testGrahamsAlgTbb(dataGrahamsAlgTbb);
  ASSERT_EQ(testGrahamsAlgTbb.validation(), true);
  testGrahamsAlgTbb.pre_processing();
  testGrahamsAlgTbb.run();
  testGrahamsAlgTbb.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i].x, mchTbb[i].x);
    ASSERT_EQ(mchSeq[i].y, mchTbb[i].y);
  }
}

// Points are located on the same straight line
TEST(savotina_v_grahams_alg_tbb, Test3) {
  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> points = {
      SavotinaTbb::SavotinaPoint(-1.4, 2.2), SavotinaTbb::SavotinaPoint(2.2, 2.2),
      SavotinaTbb::SavotinaPoint(-0.6, 2.2), SavotinaTbb::SavotinaPoint(-2.3, 2.2),
      SavotinaTbb::SavotinaPoint(2.7, 2.2),  SavotinaTbb::SavotinaPoint(0.6, 2.2),
      SavotinaTbb::SavotinaPoint(1.7, 2.2),  SavotinaTbb::SavotinaPoint(-3.4, 2.2),
      SavotinaTbb::SavotinaPoint(5.3, 2.2),  SavotinaTbb::SavotinaPoint(4.4, 2.2),
      SavotinaTbb::SavotinaPoint(3.4, 2.2)};
  std::vector<SavotinaTbb::SavotinaPoint> mchSeq(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> pointsTbb = {
      SavotinaTbb::SavotinaPoint(-1.4, 2.2), SavotinaTbb::SavotinaPoint(2.2, 2.2),
      SavotinaTbb::SavotinaPoint(-0.6, 2.2), SavotinaTbb::SavotinaPoint(-2.3, 2.2),
      SavotinaTbb::SavotinaPoint(2.7, 2.2),  SavotinaTbb::SavotinaPoint(0.6, 2.2),
      SavotinaTbb::SavotinaPoint(1.7, 2.2),  SavotinaTbb::SavotinaPoint(-3.4, 2.2),
      SavotinaTbb::SavotinaPoint(5.3, 2.2),  SavotinaTbb::SavotinaPoint(4.4, 2.2),
      SavotinaTbb::SavotinaPoint(3.4, 2.2)};
  std::vector<SavotinaTbb::SavotinaPoint> mchTbb(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(pointsTbb.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(pointsTbb.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchTbb.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mchTbb.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmTbbParallel testGrahamsAlgTbb(dataGrahamsAlgTbb);
  ASSERT_EQ(testGrahamsAlgTbb.validation(), true);
  testGrahamsAlgTbb.pre_processing();
  testGrahamsAlgTbb.run();
  testGrahamsAlgTbb.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i].x, mchTbb[i].x);
    ASSERT_EQ(mchSeq[i].y, mchTbb[i].y);
  }
}

// Only one point
TEST(savotina_v_grahams_alg_tbb, Test4) {
  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> points = {SavotinaTbb::SavotinaPoint(3.5, 4.7)};
  std::vector<SavotinaTbb::SavotinaPoint> mchSeq(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> pointsTbb = {SavotinaTbb::SavotinaPoint(3.5, 4.7)};
  std::vector<SavotinaTbb::SavotinaPoint> mchTbb(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(pointsTbb.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(pointsTbb.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchTbb.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mchTbb.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmTbbParallel testGrahamsAlgTbb(dataGrahamsAlgTbb);
  ASSERT_EQ(testGrahamsAlgTbb.validation(), true);
  testGrahamsAlgTbb.pre_processing();
  testGrahamsAlgTbb.run();
  testGrahamsAlgTbb.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i].x, mchTbb[i].x);
    ASSERT_EQ(mchSeq[i].y, mchTbb[i].y);
  }
}

// Only two points
TEST(savotina_v_grahams_alg_tbb, Test5) {
  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> points = {SavotinaTbb::SavotinaPoint(2.4, -2.9),
                                                    SavotinaTbb::SavotinaPoint(-1.8, 4.2)};
  std::vector<SavotinaTbb::SavotinaPoint> mchSeq(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> pointsTbb = {SavotinaTbb::SavotinaPoint(2.4, -2.9),
                                                       SavotinaTbb::SavotinaPoint(-1.8, 4.2)};
  std::vector<SavotinaTbb::SavotinaPoint> mchTbb(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(pointsTbb.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(pointsTbb.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchTbb.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mchTbb.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmTbbParallel testGrahamsAlgTbb(dataGrahamsAlgTbb);
  ASSERT_EQ(testGrahamsAlgTbb.validation(), true);
  testGrahamsAlgTbb.pre_processing();
  testGrahamsAlgTbb.run();
  testGrahamsAlgTbb.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i].x, mchTbb[i].x);
    ASSERT_EQ(mchSeq[i].y, mchTbb[i].y);
  }
}

// No points
TEST(savotina_v_grahams_alg_tbb, Test6) {
  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> points = {};
  std::vector<SavotinaTbb::SavotinaPoint> mchSeq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> pointsTbb = {};
  std::vector<SavotinaTbb::SavotinaPoint> mchTbb(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(pointsTbb.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(pointsTbb.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchTbb.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mchTbb.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmTbbParallel testGrahamsAlgTbb(dataGrahamsAlgTbb);
  ASSERT_EQ(testGrahamsAlgTbb.validation(), true);
  testGrahamsAlgTbb.pre_processing();
  testGrahamsAlgTbb.run();
  testGrahamsAlgTbb.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i].x, mchTbb[i].x);
    ASSERT_EQ(mchSeq[i].y, mchTbb[i].y);
  }
}

TEST(savotina_v_grahams_alg_tbb, Test7) {
  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> points = {
      SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7),
      SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7),
      SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7),
      SavotinaTbb::SavotinaPoint(4.4, 1.7)};
  std::vector<SavotinaTbb::SavotinaPoint> mchSeq(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaTbb::SavotinaPoint> pointsTbb = {
      SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7),
      SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7),
      SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7), SavotinaTbb::SavotinaPoint(4.4, 1.7),
      SavotinaTbb::SavotinaPoint(4.4, 1.7)};
  std::vector<SavotinaTbb::SavotinaPoint> mchTbb(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(pointsTbb.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(pointsTbb.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchTbb.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mchTbb.size());

  // Create Task
  SavotinaTbb::SavotinaGrahamsAlgorithmTbbParallel testGrahamsAlgTbb(dataGrahamsAlgTbb);
  ASSERT_EQ(testGrahamsAlgTbb.validation(), true);
  testGrahamsAlgTbb.pre_processing();
  testGrahamsAlgTbb.run();
  testGrahamsAlgTbb.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i].x, mchTbb[i].x);
    ASSERT_EQ(mchSeq[i].y, mchTbb[i].y);
  }
}
