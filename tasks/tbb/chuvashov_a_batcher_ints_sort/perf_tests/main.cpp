// Copyright 2024 Chuvashov Andrey
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/chuvashov_a_batcher_ints_sort/include/batcher_ints_sort.hpp"

TEST(ChuvashovABatcherPerfTestsTBBs, Test_Run_Task) {
  std::vector<int> vect = ChuvashovTBB_GenerateVector(1'000'000);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  auto testTbb = std::make_shared<Chuvashov_TBBBatcherSort>(tbbData);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(ChuvashovABatcherPerfTestsTBBs, Test_Run_Pipeline) {
  std::vector<int> vect = ChuvashovTBB_GenerateVector(1'000'000);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> tbbData = std::make_shared<ppc::core::TaskData>();
  tbbData->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  tbbData->inputs_count.emplace_back(vect.size());
  tbbData->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  tbbData->outputs_count.emplace_back(result.size());

  auto testTbb = std::make_shared<Chuvashov_TBBBatcherSort>(tbbData);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}
