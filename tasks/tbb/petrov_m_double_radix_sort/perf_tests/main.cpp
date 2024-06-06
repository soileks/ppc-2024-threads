// Copyright 2024 Petrov Maksim

#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/petrov_m_double_radix_sort/include/ops_tbb.hpp"
using namespace petrov_tbb;

TEST(Petrov_M_Radix_Sort_TBB1, test_pipeline_run) {
  std::vector<double> in(1000000);
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_real_distribution<double> distr(-10000, 100000);
  for (auto &num : in) {
    num = distr(eng);
  }
  std::vector<double> sorted = in;
  std::sort(sorted.begin(), sorted.end());
  std::vector<double> out(in.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<PetrovRadixSortDoubleTBB>(taskDataPar);
  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(sorted[i], out[i]);
  }
}

TEST(Petrov_M_Radix_Sort_TBB1, test_task_run) {
  std::vector<double> in(1000000);
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_real_distribution<double> distr(-10000, 10000);
  for (auto &num : in) {
    num = distr(eng);
  }
  std::vector<double> sorted = in;
  std::sort(sorted.begin(), sorted.end());
  std::vector<double> out(in.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskTBB = std::make_shared<PetrovRadixSortDoubleTBB>(taskDataPar);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < out.size(); i++) {
    ASSERT_EQ(sorted[i], out[i]);
  }
}
