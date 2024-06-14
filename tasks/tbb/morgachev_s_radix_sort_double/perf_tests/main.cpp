// Copyright 2024 Morgachev Stepan
#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/morgachev_s_radix_sort_double/include/ops_tbb.hpp"

using namespace morgachev_tbb;

std::vector<double> fillVectorRandomNumbers(size_t size, int min, int max) {
  std::vector<double> vector(size);
  std::mt19937 gen(1.0);
  std::uniform_real_distribution<double> dis(min, max);

  for (auto &element : vector) {
    element = dis(gen);
  }

  return vector;
}

TEST(morgachev_s_radix_sort_double_tbb, test_pipeline_run) {
  const int count = 5e+6;
  std::vector<double> in = fillVectorRandomNumbers(count, 0.0, count);
  std::vector<double> sorted = in;
  std::vector<double> out(count);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  auto testTaskTBB = std::make_shared<RadixSortDoubleTBB>(taskDataTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::sort(sorted.begin(), sorted.end());
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(out[i], sorted[i]);
  }
}

TEST(morgachev_s_radix_sort_double_tbb, test_task_run) {
  const int count = 5e+6;

  std::vector<double> in = fillVectorRandomNumbers(count, 0.0, count);
  std::vector<double> sorted = in;
  std::vector<double> out(count);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  auto testTaskTBB = std::make_shared<RadixSortDoubleTBB>(taskDataTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::sort(sorted.begin(), sorted.end());
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(sorted[i], out[i]);
  }
}
