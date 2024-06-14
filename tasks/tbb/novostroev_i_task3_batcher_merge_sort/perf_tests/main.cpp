// Copyright 2024 Novostroev Ivan
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/novostroev_i_task3_batcher_merge_sort/include/ops_tbb.hpp"

TEST(novostroev_i_batcher_sort_perf_tbb, test_pipeline_run) {
  std::vector<int> vect = RandomVector(1);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  taskDataSeq->inputs_count.emplace_back(vect.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(result.size());

  auto testOmp = std::make_shared<BatcherMergeSortTBB>(taskDataSeq);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}

TEST(novostroev_i_batcher_sort_perf_tbb, test_task_run) {
  std::vector<int> vect = RandomVector(1);
  std::vector<int> result(vect.size(), 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vect.data()));
  taskDataSeq->inputs_count.emplace_back(vect.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(result.size());

  auto testOmp = std::make_shared<BatcherMergeSortTBB>(taskDataSeq);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(std::is_sorted(result.begin(), result.end()));
}
