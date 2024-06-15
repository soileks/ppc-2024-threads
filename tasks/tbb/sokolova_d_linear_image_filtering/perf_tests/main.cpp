// Copyright 2024 Sokolova Daria
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/sokolova_d_linear_image_filtering/include/ops_tbb.hpp"

using namespace sokolova_tbb;

TEST(sokolova_d_linear_image_filtering_tbb, test_pipeline_run) {
  uint32_t width = 1024;
  uint32_t height = 1024;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 255);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 255);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  auto gaussFilterHorizontalTbb = std::make_shared<GaussFilterHorizontalTbb>(taskDataTbb);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(gaussFilterHorizontalTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(expected, out);
}

TEST(sokolova_d_linear_image_filtering_tbb, test_task_run) {
  uint32_t width = 1024;
  uint32_t height = 1024;

  // Create data
  std::vector<uint8_t> in(width * height * 3, 255);
  std::vector<uint8_t> out(width * height * 3, 0);
  std::vector<uint8_t> expected(width * height * 3, 255);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  // Create Task
  auto gaussFilterHorizontalTbb = std::make_shared<GaussFilterHorizontalTbb>(taskDataTbb);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(gaussFilterHorizontalTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(expected, out);
}
