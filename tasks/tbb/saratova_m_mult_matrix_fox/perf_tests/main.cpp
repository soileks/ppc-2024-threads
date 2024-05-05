// Copyright 2024 Saratova Marina
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/saratova_m_mult_matrix_fox/include/ops_tbb.hpp"

using namespace saratova_tbb;

TEST(Saratova_M_Mult_Matrix_Fox, test_pipeline_run) {
  size_t n = 580;
  double k = 43.0;
  std::vector<double> A(n * n);
  std::vector<double> B(n * n);
  std::vector<double> C(n * n);
  GenerateRandomValue(A.data(), A.size());
  IdentityMatrix(B.data(), n, k);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataTbb->inputs_count.emplace_back(A.size());
  taskDataTbb->inputs_count.emplace_back(B.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataTbb->outputs_count.emplace_back(C.size());

  // Create Task
  auto saratovaTaskTbb = std::make_shared<SaratovaTaskTbb>(taskDataTbb);


  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(saratovaTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(C[i], k * A[i]);
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, test_task_run) {
  size_t n = 580;
  double k = 43.0;
  std::vector<double> A(n * n);
  std::vector<double> B(n * n);
  std::vector<double> C(n * n);
  GenerateRandomValue(A.data(), A.size());
  IdentityMatrix(B.data(), n, k);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataTbb->inputs_count.emplace_back(A.size());
  taskDataTbb->inputs_count.emplace_back(B.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataTbb->outputs_count.emplace_back(C.size());

  // Create Task
  auto saratovaTaskTbb = std::make_shared<SaratovaTaskTbb>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(saratovaTaskTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(C[i], k * A[i]);
  }
}