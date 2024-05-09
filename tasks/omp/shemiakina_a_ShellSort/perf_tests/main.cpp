// Copyright 2024 Shemiakina Alesia
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/shemiakina_a_ShellSort/include/ops_omp.hpp"

TEST(shemiakina_a_ShellSort_OMP, pipeline_run) {
  const int count = 10000;

  // Create data
  std::vector<int> in = ShellTaskOMP::give_random_vector(count, 1, 100);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOMP = std::make_shared<ppc::core::TaskData>();
  taskDataOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOMP->inputs_count.emplace_back(in.size());
  taskDataOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOMP->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskOMP = std::make_shared<ShellTaskOMP>(taskDataOMP);

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
  ASSERT_TRUE(ShellTaskOMP::CheckSort(out));
}

TEST(shemiakina_a_ShellSort_OMP, task_run) {
  const int count = 100;

  // Create data
  std::vector<int> in(1, count);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOMP = std::make_shared<ppc::core::TaskData>();
  taskDataOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOMP->inputs_count.emplace_back(in.size());
  taskDataOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOMP->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskOMP = std::make_shared<ShellTaskOMP>(taskDataOMP);

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
  ASSERT_EQ(count, out[0]);
}
