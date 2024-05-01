// Copyright 2024 Bakhtiarov Alexander
#include <gtest/gtest.h>
#include <omp.h>

#include "core/perf/include/perf.hpp"
#include "omp/bakhtiarov_a_matrix_mult_css_omp/include/ccs_mat_multy.hpp"

using namespace std;

TEST(bakhtiarov_a_matrix_mult_ccs_omp, test_pipeline_run) {
  // Create data
  size_t z = 500;
  size_t x = 500;
  size_t c = 500;
  std::vector<double> lhs_in(z * x);
  for (size_t i = 0; i < z; ++i) {
    if (i % 4 == 0)
      for (size_t j = 0; j < x; ++j) {
        lhs_in[i * x + j] = 1.0;
      }
  }
  std::vector<double> rhs_in(x * c);
  for (size_t i = 0; i < x; ++i) {
    for (size_t j = 0; j < c; ++j) {
      if (j % 5 == 0) rhs_in[i * c + j] = 1.0;
    }
  }
  std::vector<double> out(z * c);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataSeq->inputs_count.emplace_back(z);
  taskDataSeq->inputs_count.emplace_back(x);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataSeq->inputs_count.emplace_back(x);
  taskDataSeq->inputs_count.emplace_back(c);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(z);
  taskDataSeq->outputs_count.emplace_back(c);

  // Create Task
  auto testTaskSeq = std::make_shared<SparseOmpMatrixMultiParallel>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSeq);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < z; ++i) {
    for (size_t j = 0; j < c; ++j) {
      if (i % 4 == 0 && j % 5 == 0)
        EXPECT_DOUBLE_EQ(out[i * c + j], q);
      else
        EXPECT_DOUBLE_EQ(out[i * c + j], 0.0);
    }
  }
}

TEST(bakhtiarov_a_matrix_mult_ccs_omp, test_task_run) {
  // Create data
  size_t z = 500;
  size_t x = 500;
  size_t c = 500;
  std::vector<double> lhs_in(z * x);
  for (size_t i = 0; i < z; ++i) {
    if (i % 4 == 0)
      for (size_t j = 0; j < x; ++j) {
        lhs_in[i * x + j] = 1.0;
      }
  }
  std::vector<double> rhs_in(x * c);
  for (size_t i = 0; i < x; ++i) {
    for (size_t j = 0; j < c; ++j) {
      if (j % 5 == 0) rhs_in[i * c + j] = 1.0;
    }
  }
  std::vector<double> out(z * c);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataSeq->inputs_count.emplace_back(z);
  taskDataSeq->inputs_count.emplace_back(x);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataSeq->inputs_count.emplace_back(x);
  taskDataSeq->inputs_count.emplace_back(c);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(z);
  taskDataSeq->outputs_count.emplace_back(c);

  // Create Task
  auto testTaskSeq = std::make_shared<SparseOmpMatrixMultiParallel>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSeq);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < z; ++i) {
    for (size_t j = 0; j < c; ++j) {
      if (i % 4 == 0 && j % 5 == 0)
        EXPECT_DOUBLE_EQ(out[i * c + j], x);
      else
        EXPECT_DOUBLE_EQ(out[i * c + j], 0.0);
    }
  }
}
