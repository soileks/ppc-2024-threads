// Copyright 2024 Shmelev Ivan
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/shmelev_i_shell_sorting_with_Batcher/include/ops_stl.hpp"

using namespace shmelev_stl;

TEST(shmelev_i_shell_sorting_with_Batcher, pipeline_run) {
  const int count = 2048;

  // Create data
  std::vector<int> input_array = create_random_sequence(count, 1, 1024);
  std::vector<int> output_array(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_array));
  taskDataPar->inputs_count.emplace_back(input_array.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_array));
  taskDataPar->outputs_count.emplace_back(output_array.size());

  // Create Task
  auto testTaskOmp = std::make_shared<ShmelevTaskStl>(taskDataPar);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(is_sorted(output_array));
}

TEST(shmelev_i_shell_sorting_with_Batcher, task_run) {
  const int count = 2048;

  // Create data
  std::vector<int> input_array = create_random_sequence(count, 1, 1024);
  std::vector<int> output_array(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_array));
  taskDataPar->inputs_count.emplace_back(input_array.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_array));
  taskDataPar->outputs_count.emplace_back(output_array.size());

  // Create Task
  auto testTaskOmp = std::make_shared<ShmelevTaskStl>(taskDataPar);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(is_sorted(output_array));
}