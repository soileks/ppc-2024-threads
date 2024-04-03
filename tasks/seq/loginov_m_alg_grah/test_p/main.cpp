// Copyright 2024 Loginov Maxim
#include <gtest/gtest.h>

#include <thread>

#include "core/perf/include/perf.hpp"
#include "seq/loginov_m_alg_grah/inc/Graham.hpp"

using namespace std::chrono_literals;

TEST(loginov_m_alg_grah_seq, test_pipeline_run) {
  std::vector<Point> points = {Point(-0.5, 2.2),  Point(1.6, 1.3),  Point(0.3, -0.6),  Point(0.1, 1.3),
                               Point(-1.8, 1.42), Point(-0.3, 0.6), Point(-0.5, -1.2), Point(1.2, -0.8),
                               Point(0.7, 0.4),   Point(1.1, 1.9),  Point(0.4, -1.2),  Point(-1.9, 0.4),
                               Point(-0.6, -0.3), Point(1.8, 0.5),  Point(-1.4, -0.7), Point(-0.9, 1.1),
                               Point(-1.2, 1.9),  Point(0.4, 2.2),  Point(1.7, -0.1)};
  std::vector<Point> res = {Point(-1.9, 0.4), Point(-1.4, -0.7), Point(-0.5, -1.2), Point(0.4, -1.2), Point(1.2, -0.8),
                            Point(1.7, -0.1), Point(1.8, 0.5),   Point(1.6, 1.3),   Point(1.1, 1.9),  Point(0.4, 2.2),
                            Point(-0.5, 2.2), Point(-1.2, 1.9),  Point(-1.8, 1.42)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  auto testGrahamSequential = std::make_shared<GrahamsAlgorithmSequential>(dataGrahamsAlgSeq);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(60ms);
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testGrahamSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}

TEST(loginov_m_alg_grah_seq, test_task_run) {
  std::vector<Point> points = {Point(-0.5, 2.2),  Point(1.6, 1.3),  Point(0.3, -0.6),  Point(0.1, 1.3),
                               Point(-1.8, 1.42), Point(-0.3, 0.6), Point(-0.5, -1.2), Point(1.2, -0.8),
                               Point(0.7, 0.4),   Point(1.1, 1.9),  Point(0.4, -1.2),  Point(-1.9, 0.4),
                               Point(-0.6, -0.3), Point(1.8, 0.5),  Point(-1.4, -0.7), Point(-0.9, 1.1),
                               Point(-1.2, 1.9),  Point(0.4, 2.2),  Point(1.7, -0.1)};
  std::vector<Point> res = {Point(-1.9, 0.4), Point(-1.4, -0.7), Point(-0.5, -1.2), Point(0.4, -1.2), Point(1.2, -0.8),
                            Point(1.7, -0.1), Point(1.8, 0.5),   Point(1.6, 1.3),   Point(1.1, 1.9),  Point(0.4, 2.2),
                            Point(-0.5, 2.2), Point(-1.2, 1.9),  Point(-1.8, 1.42)};
  std::vector<Point> mch(res.size());

  std::shared_ptr<ppc::core::TaskData> dataGrahamsAlgSeq = std::make_shared<ppc::core::TaskData>();
  dataGrahamsAlgSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  dataGrahamsAlgSeq->inputs_count.emplace_back(points.size());
  dataGrahamsAlgSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(mch.data()));
  dataGrahamsAlgSeq->outputs_count.emplace_back(mch.size());

  auto testGrahamSequential = std::make_shared<GrahamsAlgorithmSequential>(dataGrahamsAlgSeq);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(60ms);
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testGrahamSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(mch[i], res[i]);
  }
}