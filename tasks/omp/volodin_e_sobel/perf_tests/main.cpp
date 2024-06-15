// Copyright 2024 Volodin Evgeniy
#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp.h"
#include "omp/volodin_e_sobel/include/sobel.hpp"

TEST(volodin_omp_sobel_perf_test, test_pipeline_run) {
  const int width = 2048;
  const int height = 2048;

  std::vector<int> inImage(width * height, 0);

  std::default_random_engine rnd;
  std::uniform_int_distribution<> dist(0, 255);

  for (int i = 0; i < width * height; ++i) {
    inImage[i] = dist(rnd);
  }

  std::vector<int> outImage(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataOmp->inputs_count.emplace_back(width);
  taskDataOmp->inputs_count.emplace_back(height);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataOmp->outputs_count.emplace_back(width);
  taskDataOmp->outputs_count.emplace_back(height);

  auto sobelTaskOmp = std::make_shared<SobelTaskOMP>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(volodin_omp_sobel_perf_test, test_task_run) {
  const int width = 2048;
  const int height = 2048;

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
  auto sobelTaskOmp = std::make_shared<SobelTaskOMP>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}