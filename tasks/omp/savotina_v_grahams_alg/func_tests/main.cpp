// Copyright 2024 Savotina Valeria

#include <gtest/gtest.h>

#include "omp/savotina_v_grahams_alg/include/ops_omp.hpp"

// Points are arranged in a chaotic order (type of x and y is int)
TEST(savotina_v_grahams_alg_omp, Test1) {
  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> points = {
      SavotinaOmp::SavotinaPoint(-8, 4),   SavotinaOmp::SavotinaPoint(-4, 6),   SavotinaOmp::SavotinaPoint(-12, 2),
      SavotinaOmp::SavotinaPoint(-6, -2),  SavotinaOmp::SavotinaPoint(-10, -4), SavotinaOmp::SavotinaPoint(-4, 2),
      SavotinaOmp::SavotinaPoint(-6, 6),   SavotinaOmp::SavotinaPoint(-8, 8),   SavotinaOmp::SavotinaPoint(-10, 6),
      SavotinaOmp::SavotinaPoint(-8, 2),   SavotinaOmp::SavotinaPoint(-6, 2),   SavotinaOmp::SavotinaPoint(-10, 0),
      SavotinaOmp::SavotinaPoint(-14, -2), SavotinaOmp::SavotinaPoint(-16, -4), SavotinaOmp::SavotinaPoint(-14, -4)};
  std::vector<SavotinaOmp::SavotinaPoint> mchSeq(7);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> mchOmp(7);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Points are arranged in a chaotic order (type of x and y is double)
TEST(savotina_v_grahams_alg_omp, Test2) {
  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> points = {
      SavotinaOmp::SavotinaPoint(-0.5, 2.2),  SavotinaOmp::SavotinaPoint(1.6, 1.3),
      SavotinaOmp::SavotinaPoint(0.3, -0.6),  SavotinaOmp::SavotinaPoint(0.1, 1.3),
      SavotinaOmp::SavotinaPoint(-1.8, 1.42), SavotinaOmp::SavotinaPoint(-0.3, 0.6),
      SavotinaOmp::SavotinaPoint(-0.5, -1.2), SavotinaOmp::SavotinaPoint(1.2, -0.8),
      SavotinaOmp::SavotinaPoint(0.7, 0.4),   SavotinaOmp::SavotinaPoint(1.1, 1.9),
      SavotinaOmp::SavotinaPoint(0.4, -1.2),  SavotinaOmp::SavotinaPoint(-1.9, 0.4),
      SavotinaOmp::SavotinaPoint(-0.6, -0.3), SavotinaOmp::SavotinaPoint(1.8, 0.5),
      SavotinaOmp::SavotinaPoint(-1.4, -0.7), SavotinaOmp::SavotinaPoint(-0.9, 1.1),
      SavotinaOmp::SavotinaPoint(-1.2, 1.9),  SavotinaOmp::SavotinaPoint(0.4, 2.2),
      SavotinaOmp::SavotinaPoint(1.7, -0.1)};

  std::vector<SavotinaOmp::SavotinaPoint> mchSeq(13);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> mchOmp(13);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Points are located on the same straight line
TEST(savotina_v_grahams_alg_omp, Test3) {
  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> points = {
      SavotinaOmp::SavotinaPoint(-1.4, 2.2), SavotinaOmp::SavotinaPoint(2.2, 2.2),
      SavotinaOmp::SavotinaPoint(-0.6, 2.2), SavotinaOmp::SavotinaPoint(-2.3, 2.2),
      SavotinaOmp::SavotinaPoint(2.7, 2.2),  SavotinaOmp::SavotinaPoint(0.6, 2.2),
      SavotinaOmp::SavotinaPoint(1.7, 2.2),  SavotinaOmp::SavotinaPoint(-3.4, 2.2),
      SavotinaOmp::SavotinaPoint(5.3, 2.2),  SavotinaOmp::SavotinaPoint(4.4, 2.2),
      SavotinaOmp::SavotinaPoint(3.4, 2.2)};
  std::vector<SavotinaOmp::SavotinaPoint> mchSeq(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> mchOmp(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Only one point
TEST(savotina_v_grahams_alg_omp, Test4) {
  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> points = {SavotinaOmp::SavotinaPoint(3.5, 4.7)};
  std::vector<SavotinaOmp::SavotinaPoint> mchSeq(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> mchOmp(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Only two points
TEST(savotina_v_grahams_alg_omp, Test5) {
  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> points = {SavotinaOmp::SavotinaPoint(2.4, -2.9),
                                                    SavotinaOmp::SavotinaPoint(-1.8, 4.2)};
  std::vector<SavotinaOmp::SavotinaPoint> mchSeq(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> mchOmp(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// No points
TEST(savotina_v_grahams_alg_omp, Test6) {
  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> points = {};
  std::vector<SavotinaOmp::SavotinaPoint> mchSeq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> mchOmp(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

TEST(savotina_v_grahams_alg_omp, Test7) {
  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> points = {
      SavotinaOmp::SavotinaPoint(4.4, 1.7), SavotinaOmp::SavotinaPoint(4.4, 1.7), SavotinaOmp::SavotinaPoint(4.4, 1.7),
      SavotinaOmp::SavotinaPoint(4.4, 1.7), SavotinaOmp::SavotinaPoint(4.4, 1.7), SavotinaOmp::SavotinaPoint(4.4, 1.7),
      SavotinaOmp::SavotinaPoint(4.4, 1.7), SavotinaOmp::SavotinaPoint(4.4, 1.7), SavotinaOmp::SavotinaPoint(4.4, 1.7),
      SavotinaOmp::SavotinaPoint(4.4, 1.7)};
  std::vector<SavotinaOmp::SavotinaPoint> mchSeq(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<SavotinaOmp::SavotinaPoint> mchOmp(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  SavotinaOmp::SavotinaGrahamsAlgorithmOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}
