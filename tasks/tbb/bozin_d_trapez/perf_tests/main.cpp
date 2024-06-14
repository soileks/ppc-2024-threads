// Copyright 2024 Bozin Dmitry
#include <gtest/gtest.h>
#include <tbb/tbb.h>

#include <cmath>

#include "core/perf/include/perf.hpp"
#include "tbb/bozin_d_trapez/include/ops_tbb.hpp"

TEST(bozin_d_trapez_tbb, test_pipeline_run) {
  double ax = 0.0;
  double bx = 1.0;
  double ay = 0.0;
  double by = 1.0;

  int nx = 3000;
  int ny = 3000;

  double res = 0.25;

  auto mul_x_y = [](double x, double y) { return x * y; };

  // Create data
  std::vector<double> out(1, res);
  std::vector<double> in = {ax, ay, bx, by};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataSeq->inputs_count.emplace_back(n.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<Bozin_d_tbb::BozinTaskParralel>(taskDataSeq, mul_x_y);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(res, out[0]);
}

TEST(bozin_d_trapez_tbb, test_task_run) {
  double ax = 0.0;
  double bx = 1.0;
  double ay = 0.0;
  double by = 1.0;

  int nx = 3000;
  int ny = 3000;

  double res = 0.25;

  auto mul_x_y = [](double x, double y) { return x * y; };

  // Create data
  std::vector<double> out(1, res);
  std::vector<double> in = {ax, ay, bx, by};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataSeq->inputs_count.emplace_back(n.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<Bozin_d_tbb::BozinTaskParralel>(taskDataSeq, mul_x_y);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(res, out[0]);
}
