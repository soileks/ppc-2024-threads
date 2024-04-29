// Copyright 2024 Shmelev Ivan
#include <gtest/gtest.h>

#include <vector>
#include <omp.h>

#include "core/perf/include/perf.hpp"
#include "omp/shmelev_i_shell_sorting_with_Batcher/include/ops_seq.hpp"

using namespace shmelev_omp;

TEST(shmelev_i_shell_sorting_with_Batcher, pipeline_run) {
  const int count = 2048;

  // Create data
  std::vector<int> in = ShmelevTaskOmp::generate_random_vector(count, 1, 1024);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataPar->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskOmp = std::make_shared<ShmelevTaskOmp>(taskDataPar);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(ShmelevTaskOmp::sorted(out));
}

TEST(shmelev_i_shell_sorting_with_Batcher, task_run) {
  const int count = 2048;

  // Create data
  std::vector<int> in = ShmelevTaskOmp::generate_random_vector(count, 1, 1024);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataPar->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskOmp = std::make_shared<ShmelevTaskOmp>(taskDataPar);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(ShmelevTaskOmp::sorted(out));
}