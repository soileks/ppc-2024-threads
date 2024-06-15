// Copyright 2024 Vyunov Danila
#include <gtest/gtest.h>
#include <omp.h>
#include <tbb/tbb.h>

#include <algorithm>
#include <vector>

#include "./tbb/vyunov_d_hoar_merge_sort_tbb/include/hoar_merge.h"
#include "core/perf/include/perf.hpp"

TEST(vyunov_d_hoare_sort_omp, test_pipeline_run) {
  std::vector<int> inputArray = Getvec(2000);
  // Seq
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortSeq = std::make_shared<ppc::core::TaskData>();
  hoareSortSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSeq->inputs_count.emplace_back(inputArray.size());
  hoareSortSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortSeq->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq hoareSortSequential(hoareSortSeq);
  ASSERT_TRUE(hoareSortSequential.validation());
  hoareSortSequential.pre_processing();
  hoareSortSequential.run();
  hoareSortSequential.post_processing();

  // TBB
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortTBB = std::make_shared<ppc::core::TaskData>();
  hoareSortTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortTBB->inputs_count.emplace_back(inputArray.size());
  hoareSortTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortTBB->outputs_count.emplace_back(inputArray.size());

  auto hoareSortTBBParallel = std::make_shared<HoareSortTBBV>(hoareSortTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortTBBParallel);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}

TEST(vyunov_d_hoare_sort_omp, test_task_run) {
  std::vector<int> inputArray = Getvec(2000);
  // Seq
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortSeq = std::make_shared<ppc::core::TaskData>();
  hoareSortSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSeq->inputs_count.emplace_back(inputArray.size());
  hoareSortSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortSeq->outputs_count.emplace_back(inputArray.size());

  HoareSortSimpleSeq hoareSortSequential(hoareSortSeq);
  ASSERT_TRUE(hoareSortSequential.validation());
  hoareSortSequential.pre_processing();
  hoareSortSequential.run();
  hoareSortSequential.post_processing();

  // TBB
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortTBB = std::make_shared<ppc::core::TaskData>();
  hoareSortTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortTBB->inputs_count.emplace_back(inputArray.size());
  hoareSortTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortTBB->outputs_count.emplace_back(inputArray.size());

  auto hoareSortTBBParallel = std::make_shared<HoareSortTBBV>(hoareSortTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortTBBParallel);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}
