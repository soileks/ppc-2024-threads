// Copyright 2024 Kachalov Mikhail
#include <gtest/gtest.h>
#include <tbb/tbb.h>

#include <cmath>
#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/kachalov_m_int_monte/include/ops_tbb.hpp"
#define ESTIMATE 0.2

using integ_func = double (*)(double, double);
double Integral_sum(double x, double y) { return x + y; }

TEST(kachalov_m_int_monte_tbb, test_task_run) {
  double res = 8;
  integ_func f = Integral_sum;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTBB->inputs_count.emplace_back(in1.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTBB->inputs_count.emplace_back(in2.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));

  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<KachalovIntegralTBBMonteCarlo>(taskDataTBB);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_LT(std::abs(res - out[0]), ESTIMATE);
}

TEST(kachalov_m_int_monte_tbb, test_tbb_monte_carlo) {
  double res = 8;
  integ_func f = Integral_sum;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTBB->inputs_count.emplace_back(in1.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTBB->inputs_count.emplace_back(in2.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));

  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<KachalovIntegralTBBMonteCarlo>(taskDataTBB);

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
  ASSERT_LT(std::abs(res - out[0]), ESTIMATE);
}