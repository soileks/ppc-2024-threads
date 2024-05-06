// Copyright 2024 Shmelev Ivan
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/shmelev_i_shell_sorting_with_Batcher/include/ops_tbb.hpp"

using namespace shmelev_tbb;

TEST(shmelev_i_shell_sorting_with_Batcher, pipeline_run) {
  const int count = 2048;

  // Create data
  std::vector<int> in = ShmelevTaskTbb::generate_random_vector(count, 1, 1024);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataTbb->inputs_count.emplace_back(in.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataTbb->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTbb = std::make_shared<ShmelevTaskTbb>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(ShmelevTaskTbb::sorted(out));
}

TEST(shmelev_i_shell_sorting_with_Batcher, task_run) {
  const int count = 2048;

  // Create data
  std::vector<int> in = ShmelevTaskTbb::generate_random_vector(count, 1, 1024);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataTbb->inputs_count.emplace_back(in.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataTbb->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTbb = std::make_shared<ShmelevTaskTbb>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(ShmelevTaskTbb::sorted(out));
}