// Copyright 2024 Zhatkin Vyacheslav
#include <gtest/gtest.h>

#include "core/perf/include/perf.hpp"
#include "tbb/zhatkin_v_the_trapezoid_method/include/trapezoid_method_tbb.hpp"

TEST(zhatkin_v_trapezoid_tbb, test_pipeline_run) {
  double lowerx = 0.0;
  double upperx = 1.0;
  double lowery = 0.0;
  double uppery = 1.0;

  int nx = 50;
  int ny = 50;

  double res = 0.25;

  auto productXY = [](double x, double y) { return x * y; };

  // Create data
  std::vector<double> out = {1, res};
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataTBB->inputs_count.emplace_back(n.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<ZhatkinTaskTBB>(taskDataTBB, productXY);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(res, out[0]);
}

TEST(zhatkin_v_trapezoid_tbb, test_task_run) {
  double lowerx = 0.0;
  double upperx = 1.0;
  double lowery = 0.0;
  double uppery = 1.0;

  int nx = 50;
  int ny = 50;

  double res = 0.25;

  auto productXY = [](double x, double y) { return x * y; };
  // Create data
  std::vector<double> out = {1, res};
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataTBB->inputs_count.emplace_back(n.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<ZhatkinTaskTBB>(taskDataTBB, productXY);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_NEAR(res, out[0], 0.02);
}
