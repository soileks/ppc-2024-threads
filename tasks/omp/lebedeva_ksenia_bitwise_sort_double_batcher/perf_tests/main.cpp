// Copyright 2024 Lebedeva Ksenia
#include <gtest/gtest.h>

#include "core/perf/include/perf.hpp"
#include "omp/lebedeva_ksenia_bitwise_sort_double_batcher/include/ops_omp.hpp"

TEST(lebedeva_ksenia_bitwise_sort_double_batcher, test_pipeline_run) {
  // Create data
  std::vector<double> arr = LebedevaKOmp::randomVector(1000000, -1000000, 1000000);
  std::vector<double> out(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataOmp = std::make_shared<ppc::core::TaskData>();
  RSDBDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataOmp->inputs_count.emplace_back(arr.size());
  RSDBDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  RSDBDataOmp->outputs_count.emplace_back(out.size());

  std::vector<double> res(arr);
  std::sort(res.begin(), res.end());

  // Create Task
  auto testRSDBompParallel = std::make_shared<LebedevaKOmp::RadixSortDoubleWithBatcherOmpParallel>(RSDBDataOmp);

  // Create Perf attributes
  auto improvement = std::make_shared<ppc::core::improvement>();
  improvement->num_running = 10;
  improvement->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testRSDBompParallel);
  perfAnalyzer->pipeline_run(improvement, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(res[i], out[i]);
  }
}

TEST(lebedeva_ksenia_bitwise_sort_double_batcher, test_task_run) {
  // Create data
  std::vector<double> arr = LebedevaK::randomVector(1000000, -1000000, 1000000);
  std::vector<double> out(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataOmp = std::make_shared<ppc::core::TaskData>();
  RSDBDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataOmp->inputs_count.emplace_back(arr.size());
  RSDBDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  RSDBDataOmp->outputs_count.emplace_back(out.size());

  std::vector<double> res(arr);
  std::sort(res.begin(), res.end());

  // Create Task
  auto testRSDBompParallel = std::make_shared<LebedevaKOmp::RadixSortDoubleWithBatcherOmpParallel>(RSDBDataOmp);

  // Create Perf attributes
  auto improvement = std::make_shared<ppc::core::Improvement>();
  improvement->num_running = 10;
  improvement->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testRSDBompParallel);
  perfAnalyzer->task_run(improvement, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(res[i], out[i]);
  }
}