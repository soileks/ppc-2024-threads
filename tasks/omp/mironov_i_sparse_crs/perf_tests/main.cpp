// Copyright 2024 Mironov Ilya
#include <gtest/gtest.h>
#include <omp.h>

#include <algorithm>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/mironov_i_sparse_crs/include/ops_omp.hpp"

TEST(omp_mironov_i_sparse_crs_perf_test, test_pipeline_run) {
  int n = 2000;
  int m = 2000;
  int k = 2000;
  double ro = 0.005;
  std::vector<double> A(n * m, 0.0);
  MironovIOMP::genrateSparseMatrix(A.data(), A.size(), ro);
  std::vector<double> B(m * k, 0.0);
  MironovIOMP::genrateSparseMatrix(B.data(), B.size(), ro);
  std::vector<double> C(n * k, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskData->inputs_count.emplace_back(n);
  taskData->inputs_count.emplace_back(m);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskData->inputs_count.emplace_back(m);
  taskData->inputs_count.emplace_back(k);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  // Create Task
  auto testTask = std::make_shared<MironovIOMP>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };
  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTask);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(omp_mironov_i_sparse_crs_perf_test, test_task_run) {
  int n = 2000;
  int m = 2000;
  int k = 2000;
  double ro = 0.005;
  std::vector<double> A(n * m, 0.0);
  MironovIOMP::genrateSparseMatrix(A.data(), A.size(), ro);
  std::vector<double> B(m * k, 0.0);
  MironovIOMP::genrateSparseMatrix(B.data(), B.size(), ro);
  std::vector<double> C(n * k, 0.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskData->inputs_count.emplace_back(n);
  taskData->inputs_count.emplace_back(m);
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskData->inputs_count.emplace_back(m);
  taskData->inputs_count.emplace_back(k);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskData->outputs_count.emplace_back(C.size());

  // Create Task
  auto testTask = std::make_shared<MironovIOMP>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTask);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
