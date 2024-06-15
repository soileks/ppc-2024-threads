// Copyright 2024 Benduyzhko Tatiana
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <algorithm>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/benduyzhko_t_shell_batcher/include/ops_tbb.hpp"

TEST(tbbuential_benduyzhko_t_shell_batcher_perf_test, test_pipeline_run) {
  int n = 1e6;
  std::vector<int> arr(n);
  BenduyzhkoTBB::get_random_numbers(arr.data(), n, -n, n, 42);
  std::vector<int> out(arr);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->inputs_count.emplace_back(out.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto task = std::make_shared<BenduyzhkoTBB>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(task);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::sort(arr.begin(), arr.end());
  ASSERT_EQ(arr, out);
}

TEST(tbbuential_benduyzhko_t_shell_batcher_perf_test, test_task_run) {
  int n = 1e6;
  std::vector<int> arr(n);
  BenduyzhkoTBB::get_random_numbers(arr.data(), n, -n, n, 42);
  std::vector<int> out = arr;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->inputs_count.emplace_back(out.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto task = std::make_shared<BenduyzhkoTBB>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(task);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::sort(arr.begin(), arr.end());
  ASSERT_EQ(arr, out);
}
