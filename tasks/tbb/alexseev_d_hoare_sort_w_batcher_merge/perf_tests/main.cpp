// Copyright 2024 Alexseev Danila
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <algorithm>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/alexseev_d_hoare_sort_w_batcher_merge/include/hoare_sort_w_batcher_merge_tbb.hpp"

TEST(alexseev_d_hoare_sort_w_batcher_merge_tbb, test_pipeline_run) {
  std::vector<int> inputArray = generateRandomVector(1500, -1000, 1000);

  // Sequential
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortWBatcherMergeSeq = std::make_shared<ppc::core::TaskData>();
  hoareSortWBatcherMergeSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortWBatcherMergeSeq->inputs_count.emplace_back(inputArray.size());
  hoareSortWBatcherMergeSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortWBatcherMergeSeq->outputs_count.emplace_back(inputArray.size());

  HoareSortWBatcherMergeSequential hoareSortWBatcherMergeSequential(hoareSortWBatcherMergeSeq);
  ASSERT_TRUE(hoareSortWBatcherMergeSequential.validation());
  hoareSortWBatcherMergeSequential.pre_processing();
  hoareSortWBatcherMergeSequential.run();
  hoareSortWBatcherMergeSequential.post_processing();

  // TBB
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortWBatcherMergeTBB = std::make_shared<ppc::core::TaskData>();
  hoareSortWBatcherMergeTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortWBatcherMergeTBB->inputs_count.emplace_back(inputArray.size());
  hoareSortWBatcherMergeTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortWBatcherMergeTBB->outputs_count.emplace_back(inputArray.size());

  auto hoareSortWBatcherMergeTBBParallel = std::make_shared<HoareSortWBatcherMergeTBB>(hoareSortWBatcherMergeTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortWBatcherMergeTBBParallel);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}

TEST(alexseev_d_hoare_sort_w_batcher_merge_tbb, test_task_run) {
  std::vector<int> inputArray = generateRandomVector(1500, -1000, 1000);

  // Sequential
  std::vector<int> outputArraySeq(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortWBatcherMergeSeq = std::make_shared<ppc::core::TaskData>();
  hoareSortWBatcherMergeSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortWBatcherMergeSeq->inputs_count.emplace_back(inputArray.size());
  hoareSortWBatcherMergeSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArraySeq.data()));
  hoareSortWBatcherMergeSeq->outputs_count.emplace_back(inputArray.size());

  HoareSortWBatcherMergeSequential hoareSortWBatcherMergeSequential(hoareSortWBatcherMergeSeq);
  ASSERT_TRUE(hoareSortWBatcherMergeSequential.validation());
  hoareSortWBatcherMergeSequential.pre_processing();
  hoareSortWBatcherMergeSequential.run();
  hoareSortWBatcherMergeSequential.post_processing();

  // TBB
  std::vector<int> outputArrayTBB(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortWBatcherMergeTBB = std::make_shared<ppc::core::TaskData>();
  hoareSortWBatcherMergeTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortWBatcherMergeTBB->inputs_count.emplace_back(inputArray.size());
  hoareSortWBatcherMergeTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayTBB.data()));
  hoareSortWBatcherMergeTBB->outputs_count.emplace_back(inputArray.size());

  auto hoareSortWBatcherMergeTBBParallel = std::make_shared<HoareSortWBatcherMergeTBB>(hoareSortWBatcherMergeTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortWBatcherMergeTBBParallel);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(outputArraySeq, outputArrayTBB);
}
