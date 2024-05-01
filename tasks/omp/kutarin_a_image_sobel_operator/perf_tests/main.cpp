// Copyright 2024 Kutarin Alexander
#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/kutarin_a_image_sobel_operator/include/ops_omp.hpp"

TEST(kutarin_a_image_sobel_operator, test_pipeline_runparallel) {
  const int width = 512;
  const int height = 512;

  std::vector<int> inImage(width * height, 0);

  std::default_random_engine rnd;
  std::uniform_int_distribution<> dist(0, 255);

  for (int i = 0; i < width * height; ++i) {
    inImage[i] = dist(rnd);
  }

  std::vector<int> outImage(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskData_omp = std::make_shared<ppc::core::TaskData>();
  taskData_omp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskData_omp->inputs_count.emplace_back(width);
  taskData_omp->inputs_count.emplace_back(height);
  taskData_omp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskData_omp->outputs_count.emplace_back(width);
  taskData_omp->outputs_count.emplace_back(height);

  auto sobelTaskParallel = std::make_shared<SobelTaskParallel>(taskData_omp);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskParallel);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(kutarin_a_image_sobel_operator, test_task_run_parallel) {
  const int width = 512;
  const int height = 512;

  std::vector<int> inImage(width * height, 0);

  std::default_random_engine rnd;
  std::uniform_int_distribution<> dist(0, 255);

  for (int i = 0; i < width * height; ++i) {
    inImage[i] = dist(rnd);
  }

  std::vector<int> outImage(height * width, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData_omp = std::make_shared<ppc::core::TaskData>();
  taskData_omp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskData_omp->inputs_count.emplace_back(width);
  taskData_omp->inputs_count.emplace_back(height);
  taskData_omp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskData_omp->outputs_count.emplace_back(width);
  taskData_omp->outputs_count.emplace_back(height);

  // Create Task
  auto sobelTaskParallel = std::make_shared<SobelTaskParallel>(taskData_omp);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskParallel);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
