// Copyright 2024 Loginov Maxim

#include "tbb/loginov_m_alg_grah_tbb/include/ops_tbb.hpp"

#include <gtest/gtest.h>

#include "core/perf/include/perf.hpp"


TEST(loginov_m_alg_grah_tbb, test_pipeline_run) {
  double min = 0;
  double max = 100000;
  size_t size = 400000;  // size of vector of random points

  // Create data
  std::vector<LoginovTbb::LoginovPoint> points = {
      LoginovTbb::LoginovPoint(max, min), LoginovTbb::LoginovPoint(min, max), LoginovTbb::LoginovPoint(max, max),
      LoginovTbb::LoginovPoint(min, min)};
  std::vector<LoginovTbb::LoginovPoint> randomPoints = LoginovTbb::LoginovRandomPoints(min, max, size);
  points.insert(points.end(), randomPoints.begin(), randomPoints.end());
  std::vector<LoginovTbb::LoginovPoint> res = {LoginovTbb::LoginovPoint(min, min), LoginovTbb::LoginovPoint(max, min),
                                               LoginovTbb::LoginovPoint(max, max), LoginovTbb::LoginovPoint(min, max)};
  std::vector<LoginovTbb::LoginovPoint> mch(res.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(points.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mch.size());

  // Create Task
  auto testGrahamTbb = std::make_shared<LoginovTbb::LoginovGrahAlgTbbParallel>(dataGrahamsAlgTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testGrahamTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_tbb, test_task_run) {
  double min = 0;
  double max = 100000;
  size_t size = 400000;  // size of vector of random points

  // Create data
  std::vector<LoginovTbb::LoginovPoint> points = {
      LoginovTbb::LoginovPoint(max, min), LoginovTbb::LoginovPoint(min, max), LoginovTbb::LoginovPoint(max, max),
      LoginovTbb::LoginovPoint(min, min)};
  std::vector<LoginovTbb::LoginovPoint> randomPoints = LoginovTbb::LoginovRandomPoints(min, max, size);
  points.insert(points.end(), randomPoints.begin(), randomPoints.end());
  std::vector<LoginovTbb::LoginovPoint> res = {LoginovTbb::LoginovPoint(min, min), LoginovTbb::LoginovPoint(max, min),
                                               LoginovTbb::LoginovPoint(max, max), LoginovTbb::LoginovPoint(min, max)};
  std::vector<LoginovTbb::LoginovPoint> mch(res.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgTbb = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgTbb->inputs_count.emplace_back(points.size());
  dataGrahamsAlgTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgTbb->outputs_count.emplace_back(mch.size());

  // Create Task
  auto testGrahamTbb = std::make_shared<LoginovTbb::LoginovGrahAlgTbbParallel>(dataGrahamsAlgTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testGrahamTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}
