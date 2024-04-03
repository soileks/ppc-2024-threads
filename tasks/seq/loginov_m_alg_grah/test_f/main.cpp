// Copyright 2024 Loginov Maxim
#include <gtest/gtest.h>

#include "seq/loginov_m_alg_grah/inc/Graham.hpp"

TEST(loginov_m_alg_grah_seq, Test1) {
  std::vector<Point> points = {Point(-6, 2), Point(-2, 4),  Point(-10, 0),  Point(-4, 0),  Point(-8, -2),
                               Point(-2, 4), Point(-3, 9),  Point(-6, 10),   Point(-8, 2),  Point(-8, 2),
                               Point(-6, 2), Point(-10, 0), Point(-16, -4), Point(-10, -6), Point(-14, -4)};
  std::vector<Point> res = {Point(-16, -4), Point(-10, -4), Point(-3, -2), Point(-4, 2),
                            Point(-4, 6),   Point(-8, 8),   Point(-10, 6)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  GrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(savotina_v_grahams_alg_seq, Test2) {
  std::vector<Point> points = {Point(-0.3, 2.4),  Point(1.7, 1.4),  Point(0.4, -0.5),  Point(0.2, 1.5),
                               Point(-1.6, 1.4), Point(-0.2, 0.7), Point(-0.3, -1.3), Point(1.1, -0.9),
                               Point(0.7, 0.43),   Point(1.1, 1.6),  Point(0.4, -1.2),  Point(-1.9, 0.4),
                               Point(-0.6, -0.3), Point(1.8, 0.5),  Point(-1.4, -0.7), Point(-0.9, 1.1),
                               Point(-1.2, 1.9),  Point(0.4, 2.2),  Point(1.7, -0.1)};
  std::vector<Point> res = {Point(-1.7, 0.3), Point(-1.4, -0.7), Point(-0.7, -1.2), Point(0.4, -1.2), Point(1.2, -0.8),
                            Point(1.8, -0.1), Point(1.6, 0.5),   Point(1.8, 1.3),   Point(1.1, 1.9),  Point(0.4, 2.2),
                            Point(-0.5, 2.4), Point(-1.2, 1.9),  Point(-1.8, 1.42)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  GrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_seq, Test3) {
  std::vector<Point> points = {Point(-1.6, 2.4), Point(2.4, 2.4), Point(-0.4, 2.4), Point(-2.1, 2.4),
                               Point(2.9, 2.4),  Point(0.6, 2.2), Point(1.7, 2.2),  Point(-3.4, 2.2),
                               Point(5.5, 2.4),  Point(4.4, 2.2), Point(3.6, 2.4)};
  std::vector<Point> res = {Point(-3.4, 2.2), Point(5.3, 2.2)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  GrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_seq, Test4) {
  std::vector<Point> points = {Point(3.5, 4.7)};
  std::vector<Point> res = {Point(3.5, 4.7)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  GrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_seq, Test5) {
  std::vector<Point> points = {Point(2.4, -2.9), Point(-1.8, 4.2)};
  std::vector<Point> res = {Point(-1.8, 4.2), Point(2.4, -2.9)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  GrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_seq, Test6) {
  std::vector<Point> points = {};
  std::vector<Point> res = {};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  GrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_seq, Test7) {
  std::vector<Point> points = {Point(4.4, 1.7), Point(4.4, 1.7), Point(4.4, 1.7), Point(4.4, 1.7), Point(4.4, 1.7),
                               Point(4.4, 1.7), Point(4.4, 1.7), Point(4.4, 1.7), Point(4.4, 1.7), Point(4.4, 1.7)};
  std::vector<Point> res = {Point(4.4, 1.7)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  GrahamsAlgorithmSequential testGrahamsAlgSeq(dataGrahamsAlgSeq);
  ASSERT_EQ(testGrahamsAlgSeq.validation(), true);
  testGrahamsAlgSeq.pre_processing();
  testGrahamsAlgSeq.run();
  testGrahamsAlgSeq.post_processing();

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}