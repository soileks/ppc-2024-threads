// Copyright 2024 Vyunov Danila
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "./seq/vyunov_d_hoar_merge_sort/include/hoar_merge.h"
#include "core/perf/include/perf.hpp"

TEST(vyunov_d_hoare_sort_seq, test_pipeline_run) {
  std::vector<int> inputArray = Getvec(2000);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  auto hoareSortSeq = std::make_shared<HoareSortSimpleSeq>(hoareSortSequential);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortSeq);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(expectedArray, outputArray);
}

TEST(vyunov_d_hoare_sort_seq, test_task_run) {
  std::vector<int> inputArray = Getvec(2000);
  std::vector<int> sortedInputArray = inputArray;
  std::sort(sortedInputArray.begin(), sortedInputArray.end());
  std::vector<int> outputArray(inputArray.size());
  std::vector<int> expectedArray = sortedInputArray;

  std::shared_ptr<ppc::core::TaskData> hoareSortSequential = std::make_shared<ppc::core::TaskData>();
  hoareSortSequential->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputArray.data()));
  hoareSortSequential->inputs_count.emplace_back(inputArray.size());
  hoareSortSequential->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputArray.data()));
  hoareSortSequential->outputs_count.emplace_back(inputArray.size());

  auto hoareSortSeq = std::make_shared<HoareSortSimpleSeq>(hoareSortSequential);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(hoareSortSeq);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(expectedArray, outputArray);
}
