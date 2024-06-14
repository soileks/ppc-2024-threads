// Copyright 2024 Kachalov Mikhail
#include <gtest/gtest.h>

#include <cmath>
#include <random>
#include <thread>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/kachalov_m_int_monte/include/ops_stl.hpp"
#define ESTIMATE 0.2

using integ_func = double (*)(double, double);
double Integral_sum(double x, double y) { return x + y; }

TEST(kachalov_m_int_monte_stl, test_stl_monte_carlo) {
  double res = 8;
  integ_func f = Integral_sum;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSTL->inputs_count.emplace_back(in1.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSTL->inputs_count.emplace_back(in2.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));

  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSTL = std::make_shared<KachalovIntegralSTLMonteCarlo>(taskDataSTL);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSTL);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_LT(std::abs(res - out[0]), ESTIMATE);
}

TEST(kachalov_m_int_monte_stl, test_task_run) {
  double res = 8;
  integ_func f = Integral_sum;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSTL->inputs_count.emplace_back(in1.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSTL->inputs_count.emplace_back(in2.size());
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));

  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSTL = std::make_shared<KachalovIntegralSTLMonteCarlo>(taskDataSTL);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSTL);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_LT(std::abs(res - out[0]), ESTIMATE);
}