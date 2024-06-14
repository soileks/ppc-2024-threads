// Copyright 2024 Platonova Mariya
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/platonova_m_jarvis_stl/include/ops_stl.hpp"

TEST(Platonova_m_jarvis, test_pipeline_run) {
  std::vector<Point> points;
  const int size = 512000;
  points.reserve(size);
  for (int i = 0; i < size; i++) {
    points.push_back({i % 100, i % 200});
  }
  std::vector<Point> res(points.size());
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> testDataStl = std::make_shared<ppc::core::TaskData>();
  testDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  testDataStl->inputs_count.emplace_back(points.size());
  testDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  testDataStl->outputs_count.emplace_back(res.size());

  // Create Task
  auto testTaskOMP = std::make_shared<JarvisSTL>(testDataStl);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(Platonova_m_jarvis, test_task_run) {
  std::vector<Point> points;
  const int size = 512000;
  points.reserve(size);
  for (int i = 0; i < size; i++) {
    points.push_back({rand() % 100, rand() % 200});
  }

  std::vector<Point> res(points.size());
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> testDataStl = std::make_shared<ppc::core::TaskData>();
  testDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  testDataStl->inputs_count.emplace_back(points.size());
  testDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  testDataStl->outputs_count.emplace_back(res.size());

  // Create Task
  auto testTaskOMP = std::make_shared<JarvisSTL>(testDataStl);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
