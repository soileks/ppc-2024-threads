// Copyright 2024 Kutarin Alexander
#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/kutarin_a_image_sobel_operator/include/ops_omp.hpp"

TEST(kutarin_a_sobel_omp, test_pipeline_run) {
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
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  auto sobelTaskOmp = std::make_shared<KutarinASobel>(taskDataOmp);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(kutarin_a_sobel_omp, test_task_run) {
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
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  // Create Task
  auto sobelTaskOmp = std::make_shared<KutarinASobel>(taskDataOmp);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
