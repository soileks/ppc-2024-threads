// Copyright 2024 Zhatkin Vyacheslav
#include <gtest/gtest.h>
#include <omp.h>

#include "core/perf/include/perf.hpp"
#include "omp/zhatkin_v_the_trapezoid_method/include/trapezoid_method_omp.hpp"

TEST(zhatkin_v_trapezoid_omp, test_pipeline_run) {
  double lowerx = 0.0;
  double upperx = 1.0;
  double lowery = 0.0;
  double uppery = 1.0;

  int nx = 50;
  int ny = 50;

  double res = 0.25;

  auto mul_x_y = [](double x, double y) { return x * y; };

  // Create data
  std::vector<double> out = {1, res};
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
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
  auto testTaskOMP = std::make_shared<ZhatkinTaskOMP>(taskDataSeq, mul_x_y);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(res, out[0]);
}

TEST(zhatkin_v_trapezoid_omp, test_task_run) {
  double lowerx = 0.0;
  double upperx = 1.0;
  double lowery = 0.0;
  double uppery = 1.0;

  int nx = 50;
  int ny = 50;

  double res = 0.25;

  auto mul_x_y = [](double x, double y) { return x * y; };
  // Create data
  std::vector<double> out = {1, res};
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
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
  auto testTaskOMP = std::make_shared<ZhatkinTaskOMP>(taskDataSeq, mul_x_y);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_NEAR(res, out[0], 0.02);
}
