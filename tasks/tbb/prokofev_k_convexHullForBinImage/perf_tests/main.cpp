// Copyright 2024 Prokofev Kirill
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/prokofev_k_convexHullForBinImage/include/ops_tbb.hpp"

TEST(prokofev_k_convex_hull_tbb, test_pipeline_tbb) {
  // Create data
  int countComp = 2000;
  int width = 8;
  int height = 8 * countComp + countComp;
  std::vector<int> out(9 * countComp);
  std::vector<int> vecImage = prokofev_k_convexHull_Tbb::GenerateImgForPerfTests(countComp);
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(out.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  auto testTaskTbb = std::make_shared<prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(prokofev_k_convex_hull_tbb, test_task_run) {
  // Create data
  int countComp = 2000;
  int width = 8;
  int height = 8 * countComp + countComp;
  std::vector<int> out(9 * countComp);
  std::vector<int> vecImage = prokofev_k_convexHull_Tbb::GenerateImgForPerfTests(countComp);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(out.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  auto testTaskTbb = std::make_shared<prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}