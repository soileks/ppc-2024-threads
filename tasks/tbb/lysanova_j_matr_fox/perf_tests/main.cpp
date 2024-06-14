// Copyright 2024 Lysanova Julia
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/lysanova_j_matr_fox/include/ops_tbb.hpp"

namespace me_tbb = lysanova_j_matr_fox_tbb;

TEST(lysanova_j_matr_fox_tbb, test_pipeline_run) {
  size_t n = 640;
  double k = 42.0;
  std::vector<double> A(n * n);
  std::vector<double> B(n * n);
  std::vector<double> C(n * n);
  me_tbb::RandomFillMatrix(A.data(), A.size());
  me_tbb::EMatrix(B.data(), n, k);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataOmp->inputs_count.emplace_back(A.size());
  taskDataOmp->inputs_count.emplace_back(B.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataOmp->outputs_count.emplace_back(C.size());

  // Create Task
  auto lysanovaTaskOmp = std::make_shared<LysanovaTaskOmp>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(lysanovaTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(C[i], k * A[i]);
  }
}

TEST(lysanova_j_matr_fox_tbb, test_task_run) {
  size_t n = 640;
  double k = 42.0;
  std::vector<double> A(n * n);
  std::vector<double> B(n * n);
  std::vector<double> C(n * n);
  me_tbb::RandomFillMatrix(A.data(), A.size());
  me_tbb::EMatrix(B.data(), n, k);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataOmp->inputs_count.emplace_back(A.size());
  taskDataOmp->inputs_count.emplace_back(B.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(C.data()));
  taskDataOmp->outputs_count.emplace_back(C.size());

  // Create Task
  auto lysanovaTaskOmp = std::make_shared<LysanovaTaskOmp>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(lysanovaTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(C[i], k * A[i]);
  }
}
