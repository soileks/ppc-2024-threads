// Copyright 2024 Volodin Evgeniy
#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/volodin_e_sobel/include/sobel.hpp"

TEST(volodin_e_sobel_stl, test_pipeline_run) {
  const int width = 2048;
  const int height = 2048;

  std::vector<int> inImage(width * height, 0);

  std::default_random_engine rnd;
  std::uniform_int_distribution<> dist(0, 255);

  for (int i = 0; i < width * height; ++i) {
    inImage[i] = dist(rnd);
  }

  std::vector<int> outImage(height * width, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataStl->inputs_count.emplace_back(width);
  taskDataStl->inputs_count.emplace_back(height);
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataStl->outputs_count.emplace_back(width);
  taskDataStl->outputs_count.emplace_back(height);

  auto sobelTaskStl = std::make_shared<SobelTaskStlVolodin>(taskDataStl);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskStl);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(volodin_e_sobel_stl, test_task_run) {
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
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataStl->inputs_count.emplace_back(width);
  taskDataStl->inputs_count.emplace_back(height);
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataStl->outputs_count.emplace_back(width);
  taskDataStl->outputs_count.emplace_back(height);

  // Create Task
  auto sobelTaskStl = std::make_shared<SobelTaskStlVolodin>(taskDataStl);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(sobelTaskStl);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
