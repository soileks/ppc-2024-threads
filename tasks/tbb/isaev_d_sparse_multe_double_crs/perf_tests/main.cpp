// Copyright 2024 Isaev Dmitriy
#include <gtest/gtest.h>
#include <tbb/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/isaev_d_sparse_multe_double_crs/include/ops_tbb.hpp"

TEST(isaev_d_sparse_multe_double_crs_tbb, test_pipeline_run) {
  int N = 500;
  int M = 500;

  // Create data
  IsaevTBB::SparseMatrix A = IsaevTBB::getRandomMatrix(N, M, 0.50, 3200);
  IsaevTBB::SparseMatrix B = IsaevTBB::getRandomMatrix(M, N, 0.50, 1350);
  IsaevTBB::SparseMatrix C;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataTBB->inputs_count.emplace_back(N);
  taskDataTBB->inputs_count.emplace_back(M);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataTBB->inputs_count.emplace_back(M);
  taskDataTBB->inputs_count.emplace_back(N);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(&C));
  taskDataTBB->outputs_count.emplace_back(M);
  taskDataTBB->outputs_count.emplace_back(N);

  // Create Task
  auto taskTBB = std::make_shared<IsaevTBB::SparseMultDoubleCRStbbParallel>(taskDataTBB);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(isaev_d_sparse_multe_double_crs_tbb, test_task_run) {
  int N = 500;
  int M = 500;
  // Create data
  IsaevTBB::SparseMatrix A = IsaevTBB::getRandomMatrix(N, M, 0.50, 3200);
  IsaevTBB::SparseMatrix B = IsaevTBB::getRandomMatrix(M, N, 0.50, 1350);
  IsaevTBB::SparseMatrix C;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataTBB->inputs_count.emplace_back(N);
  taskDataTBB->inputs_count.emplace_back(M);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataTBB->inputs_count.emplace_back(N);
  taskDataTBB->inputs_count.emplace_back(M);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(&C));
  taskDataTBB->outputs_count.emplace_back(M);
  taskDataTBB->outputs_count.emplace_back(N);

  // Create Task
  auto taskTBB = std::make_shared<IsaevTBB::SparseMultDoubleCRStbbParallel>(taskDataTBB);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}