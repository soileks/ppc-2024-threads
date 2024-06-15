// Copyright 2024 Prokofev Kirill
#include <gtest/gtest.h>
#include <omp.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/prokofev_k_convexHullForBinImage/include/ops_omp.hpp"

TEST(prokofev_k_convex_hull_omp, test_pipeline_omp) {
  // Create data
  int countComp = 2000;
  int width = 8;
  int height = 8 * countComp + countComp;
  std::vector<int> out(9 * countComp);
  std::vector<int> vecImage = prokofev_k_covexHull_Omp::GenerateImgForPerfTests(countComp);
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->inputs_count.emplace_back(out.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  auto testTaskOmp = std::make_shared<prokofev_k_covexHull_Omp::BinaryImageConvexHullOmp>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(prokofev_k_convex_hull_omp, test_task_run) {
  // Create data
  int countComp = 2000;
  int width = 8;
  int height = 8 * countComp + countComp;
  std::vector<int> out(9 * countComp);
  std::vector<int> vecImage = prokofev_k_covexHull_Omp::GenerateImgForPerfTests(countComp);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->inputs_count.emplace_back(out.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  auto testTaskOmp = std::make_shared<prokofev_k_covexHull_Omp::BinaryImageConvexHullOmp>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}