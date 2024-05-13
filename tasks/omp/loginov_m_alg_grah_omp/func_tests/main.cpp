// Copyright 2024 Loginov Maxim

#include <gtest/gtest.h>

#include "omp/loginov_m_alg_grah_omp/include/ops_omp.hpp"

// Points are arranged in a chaotic order (type of x and y is int)
TEST(loginov_m_alg_grah_omp, Test1) {
  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {
      LoginovOmp::LoginovPoint(4, -7),   LoginovOmp::LoginovPoint(12, 5),   LoginovOmp::LoginovPoint(-3, 8),
      LoginovOmp::LoginovPoint(9, -2),   LoginovOmp::LoginovPoint(-6, 10),  LoginovOmp::LoginovPoint(1, -9),
      LoginovOmp::LoginovPoint(7, 3),    LoginovOmp::LoginovPoint(-5, 11),  LoginovOmp::LoginovPoint(6, -4),
      LoginovOmp::LoginovPoint(2, -8),   LoginovOmp::LoginovPoint(-1, 0),   LoginovOmp::LoginovPoint(13, -11),
      LoginovOmp::LoginovPoint(-10, 14), LoginovOmp::LoginovPoint(-13, 15), LoginovOmp::LoginovPoint(16, -14)};
  std::vector<LoginovOmp::LoginovPoint> mchSeq(7);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<LoginovOmp::LoginovPoint> mchOmp(7);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Points are arranged in a chaotic order (type of x and y is double)
TEST(loginov_m_alg_grah_omp, Test2) {
  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {
      LoginovOmp::LoginovPoint(-0.7, 2.4),  LoginovOmp::LoginovPoint(1.8, 1.1),   LoginovOmp::LoginovPoint(0.5, -0.8),
      LoginovOmp::LoginovPoint(0.3, 1.6),   LoginovOmp::LoginovPoint(-1.7, 1.43), LoginovOmp::LoginovPoint(-0.2, 0.4),
      LoginovOmp::LoginovPoint(-0.3, -1.5), LoginovOmp::LoginovPoint(1.4, -0.6),  LoginovOmp::LoginovPoint(0.8, 0.5),
      LoginovOmp::LoginovPoint(1.3, 1.5),   LoginovOmp::LoginovPoint(0.3, -1.1),  LoginovOmp::LoginovPoint(-1.7, 0.6),
      LoginovOmp::LoginovPoint(-0.7, -0.5), LoginovOmp::LoginovPoint(1.5, 0.7),   LoginovOmp::LoginovPoint(-1.7, -0.5),
      LoginovOmp::LoginovPoint(-0.7, 1.3),  LoginovOmp::LoginovPoint(-1.3, 1.5),  LoginovOmp::LoginovPoint(0.2, 2.4),
      LoginovOmp::LoginovPoint(1.9, -0.3)};

  std::vector<LoginovOmp::LoginovPoint> mchSeq(13);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<LoginovOmp::LoginovPoint> mchOmp(13);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Points are located on the same straight line
TEST(loginov_m_alg_grah_omp, Test3) {
  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {
      LoginovOmp::LoginovPoint(-1.4, 2.2), LoginovOmp::LoginovPoint(2.2, 2.2),  LoginovOmp::LoginovPoint(-0.6, 2.2),
      LoginovOmp::LoginovPoint(-2.3, 2.2), LoginovOmp::LoginovPoint(2.7, 2.2),  LoginovOmp::LoginovPoint(0.6, 2.2),
      LoginovOmp::LoginovPoint(1.7, 2.2),  LoginovOmp::LoginovPoint(-3.4, 2.2), LoginovOmp::LoginovPoint(5.3, 2.2),
      LoginovOmp::LoginovPoint(4.4, 2.2),  LoginovOmp::LoginovPoint(3.4, 2.2)};
  std::vector<LoginovOmp::LoginovPoint> mchSeq(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<LoginovOmp::LoginovPoint> mchOmp(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Only one point
TEST(loginov_m_alg_grah_omp, Test4) {
  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {LoginovOmp::LoginovPoint(3.5, 4.7)};
  std::vector<LoginovOmp::LoginovPoint> mchSeq(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<LoginovOmp::LoginovPoint> mchOmp(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// Only two points
TEST(loginov_m_alg_grah_omp, Test5) {
  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {LoginovOmp::LoginovPoint(2.4, -2.9),
                                                  LoginovOmp::LoginovPoint(-1.8, 4.2)};
  std::vector<LoginovOmp::LoginovPoint> mchSeq(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<LoginovOmp::LoginovPoint> mchOmp(2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

// No points
TEST(loginov_m_alg_grah_omp, Test6) {
  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {};
  std::vector<LoginovOmp::LoginovPoint> mchSeq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<LoginovOmp::LoginovPoint> mchOmp(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}

TEST(loginov_m_alg_grah_omp, Test7) {
  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {
      LoginovOmp::LoginovPoint(4.4, 1.7), LoginovOmp::LoginovPoint(4.4, 1.7), LoginovOmp::LoginovPoint(4.4, 1.7),
      LoginovOmp::LoginovPoint(4.4, 1.7), LoginovOmp::LoginovPoint(4.4, 1.7), LoginovOmp::LoginovPoint(4.4, 1.7),
      LoginovOmp::LoginovPoint(4.4, 1.7), LoginovOmp::LoginovPoint(4.4, 1.7), LoginovOmp::LoginovPoint(4.4, 1.7),
      LoginovOmp::LoginovPoint(4.4, 1.7)};
  std::vector<LoginovOmp::LoginovPoint> mchSeq(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchSeq.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mchSeq.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  // Create data
  std::vector<LoginovOmp::LoginovPoint> mchOmp(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mchOmp.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mchOmp.size());

  // Create Task
  LoginovOmp::LoginovGrahAlgOmpParallel testGrahamsAlgOmp(dataGrahamsAlgOmp);
  ASSERT_EQ(testGrahamsAlgOmp.validation(), true);
  testGrahamsAlgOmp.pre_processing();
  testGrahamsAlgOmp.run();
  testGrahamsAlgOmp.post_processing();

  for (size_t i = 0; i < mchSeq.size(); ++i) {
    ASSERT_EQ(mchSeq[i], mchOmp[i]);
  }
}