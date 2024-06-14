// Copyright 2024 Loginov Maxim

#include <gtest/gtest.h>

#include "core/perf/include/perf.hpp"
#include "omp/loginov_m_alg_grah_omp/include/ops_omp.hpp"

TEST(loginov_m_alg_grah_omp, test_pipeline_run) {
  double min = 0;
  double max = 100000;
  size_t size = 400000;  // size of vector of random points

  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {
      LoginovOmp::LoginovPoint(max, min), LoginovOmp::LoginovPoint(min, max), LoginovOmp::LoginovPoint(max, max),
      LoginovOmp::LoginovPoint(min, min)};
  std::vector<LoginovOmp::LoginovPoint> randomPoints = LoginovOmp::LoginovRandomPoints(min, max, size);
  points.insert(points.end(), randomPoints.begin(), randomPoints.end());
  std::vector<LoginovOmp::LoginovPoint> res = {LoginovOmp::LoginovPoint(min, min), LoginovOmp::LoginovPoint(max, min),
                                               LoginovOmp::LoginovPoint(max, max), LoginovOmp::LoginovPoint(min, max)};
  std::vector<LoginovOmp::LoginovPoint> mch(res.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mch.size());

  // Create Task
  auto testGrahamOmp = std::make_shared<LoginovOmp::LoginovGrahAlgOmpParallel>(dataGrahamsAlgOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testGrahamOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_omp, test_task_run) {
  double min = 0;
  double max = 100000;
  size_t size = 400000;  // size of vector of random points

  // Create data
  std::vector<LoginovOmp::LoginovPoint> points = {
      LoginovOmp::LoginovPoint(max, min), LoginovOmp::LoginovPoint(min, max), LoginovOmp::LoginovPoint(max, max),
      LoginovOmp::LoginovPoint(min, min)};
  std::vector<LoginovOmp::LoginovPoint> randomPoints = LoginovOmp::LoginovRandomPoints(min, max, size);
  points.insert(points.end(), randomPoints.begin(), randomPoints.end());
  std::vector<LoginovOmp::LoginovPoint> res = {LoginovOmp::LoginovPoint(min, min), LoginovOmp::LoginovPoint(max, min),
                                               LoginovOmp::LoginovPoint(max, max), LoginovOmp::LoginovPoint(min, max)};
  std::vector<LoginovOmp::LoginovPoint> mch(res.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgOmp = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgOmp->inputs_count.emplace_back(points.size());
  dataGrahamsAlgOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgOmp->outputs_count.emplace_back(mch.size());

  // Create Task
  auto testGrahamOmp = std::make_shared<LoginovOmp::LoginovGrahAlgOmpParallel>(dataGrahamsAlgOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testGrahamOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}