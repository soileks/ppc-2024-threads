// Copyright 2024 Kutarin Alexander
#include <gtest/gtest.h>

#include <random>
#include <vector>
#include <chrono>

#include "core/perf/include/perf.hpp"
#include "stl/kutarin_a_sobel/include/ops_stl.hpp"

TEST(kutarin_a_sobel_stl, test_pipeline_run) {
  const int width = 256;
  const int height = 256;

  std::vector<int> inImage(width * height, 0);

  std::default_random_engine rnd;
  std::uniform_int_distribution<> dist(0, 255);

  for (int i = 0; i < width * height; ++i) {
    inImage[i] = dist(rnd);
  }

  std::vector<int> outImage(height * width, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  auto sobelTaskTbb = std::make_shared<KutarinASobel>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] { return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(kutarin_a_sobel_stl, test_task_run) {
  const int width = 256;
  const int height = 256;

  std::vector<int> inImage(width * height, 0);

  std::default_random_engine rnd;
  std::uniform_int_distribution<> dist(0, 255);

  for (int i = 0; i < width * height; ++i) {
    inImage[i] = dist(rnd);
  }

  std::vector<int> outImage(height * width, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  auto sobelTaskTbb = std::make_shared<KutarinASobel>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] { return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
