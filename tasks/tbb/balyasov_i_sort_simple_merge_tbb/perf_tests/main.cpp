// Copyright 2024 Balyasov Ivan
#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/balyasov_i_sort_simple_merge_tbb/include/ops_tbb.hpp"

std::vector<int> genRandVector(int size) {
  std::vector<int> randomVector(size);
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < size; i++) {
    randomVector[i] = gen() % 1000;
  }
  return randomVector;
}

TEST(balyasov_i_sort_simple_merge_tbb, test_pipeline_run) {
  const int count = 5000000;

  // Create data
  std::vector<int> in = genRandVector(count);
  std::vector<int> answer = in;
  std::vector<int> out(count);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSequential = std::make_shared<RadixSortClassTaskTbb>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto time_start = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - time_start).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::sort(answer.begin(), answer.end());
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], answer[i]);
  }
}

TEST(balyasov_i_sort_simple_merge_tbb, test_task_run) {
  const int count = 5000000;

  // Create data
  std::vector<int> in = genRandVector(count);
  std::vector<int> answer = in;
  std::vector<int> out(count);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSequential = std::make_shared<RadixSortClassTaskTbb>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto time_start = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - time_start).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::sort(answer.begin(), answer.end());
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], answer[i]);
  }
}
