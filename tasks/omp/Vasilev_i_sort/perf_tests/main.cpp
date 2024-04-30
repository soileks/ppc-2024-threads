// Copyright 2024 Vasilev Ivan
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/Vasilev_i_sort/include/ops_omp.hpp"

TEST(vasilev_i_sort_omp, pipeline_run) {
  const int count = 32768;

  // Create data
  std::vector<int> in = VasilevTaskOmp::generate_random_vector(count, 1, 32768);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOMP = std::make_shared<ppc::core::TaskData>();
  taskDataOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataOMP->inputs_count.emplace_back(in.size());
  taskDataOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataOMP->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskOmp = std::make_shared<VasilevTaskOmp>(taskDataOMP);

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
  ASSERT_TRUE(VasilevTaskOmp::R_sorted(out));
}

TEST(vasilev_i_sort_omp, task_run) {
  const int count = 1048576;

  // Create data
  std::vector<int> in = VasilevTaskOmp::generate_random_vector(count, 1, 1048576);
  std::vector<int> out(count, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOMP = std::make_shared<ppc::core::TaskData>();
  taskDataOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataOMP->inputs_count.emplace_back(in.size());
  taskDataOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));
  taskDataOMP->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskOmp = std::make_shared<VasilevTaskOmp>(taskDataOMP);

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
  ASSERT_TRUE(VasilevTaskOmp::R_sorted(out));
}