// Copyright 2024 Vyunov Danila
#include <gtest/gtest.h>
#include <omp.h>

#include <algorithm>
#include <vector>
#include "./omp/vyunov_d_hoar_merge_sort_omp/include/hoar_merge.h"
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

  // OMP
  std::vector<int> outputArrayOMP(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortOMP = std::make_shared<ppc::core::TaskData>();
  hoareSortOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortOMP->inputs_count.emplace_back(inputArray.size());
  hoareSortOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayOMP.data()));
  hoareSortOMP->outputs_count.emplace_back(inputArray.size());

  auto hoareSortOMPParallel = std::make_shared<HoareSortOMP>(hoareSortOMP);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortOMPParallel);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(outputArraySeq, outputArrayOMP);
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

  // OMP
  std::vector<int> outputArrayOMP(inputArray.size());

  std::shared_ptr<ppc::core::TaskData> hoareSortOMP = std::make_shared<ppc::core::TaskData>();
  hoareSortOMP->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortOMP->inputs_count.emplace_back(inputArray.size());
  hoareSortOMP->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArrayOMP.data()));
  hoareSortOMP->outputs_count.emplace_back(inputArray.size());

  auto hoareSortOMPParallel = std::make_shared<HoareSortOMP>(hoareSortOMP);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortOMPParallel);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(outputArraySeq, outputArrayOMP);
}