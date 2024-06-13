// Copyright 2024 Martynov Aleksandr
#include <gtest/gtest.h>
#include <tbb/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/martynov_a_strassen_algorithm/include/ops_tbb.hpp"

TEST(martynov_a_strassen_alg_omp_perf, test_pipeline_run) {
  const int n = 64;
  int m = n * n;
  // Create data
  std::vector<double> first_matrix = fillMatrix(n);
  std::vector<double> second_matrix = fillMatrix(n);
  std::vector<double> result(m);
  std::vector<double> res = ijkalgorithm(first_matrix, second_matrix, n);
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_matrix.data()));
  taskDataTBB->inputs_count.emplace_back(first_matrix.size());

  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_matrix.data()));
  taskDataTBB->inputs_count.emplace_back(second_matrix.size());

  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));

  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataTBB->outputs_count.emplace_back(result.size());

  auto testTaskTBB = std::make_shared<Strssn_alg>(taskDataSeq);
  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); i++) {
    ASSERT_DOUBLE_EQ(std::round(res[i]), std::round(result[i]));
  }
}

TEST(martynov_a_strassen_alg_omp_perf, test_task_run) {
  const int n = 64;
  int m = n * n;
  // Create data
  std::vector<double> result(m);
  std::vector<double> first_matrix = fillMatrix(n);
  std::vector<double> second_matrix = fillMatrix(n);
  std::vector<double> res = ijkalgorithm(first_matrix, second_matrix, n);
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_matrix.data()));
  taskDataTBB->inputs_count.emplace_back(first_matrix.size());

  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_matrix.data()));
  taskDataTBB->inputs_count.emplace_back(second_matrix.size());

  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));

  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataTBB->outputs_count.emplace_back(result.size());

  auto testTaskTBB = std::make_shared<Strssn_alg>(taskDataTBB);
  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskDataTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); i++) {
    ASSERT_DOUBLE_EQ(std::round(res[i]), std::round(result[i]));
  }
}