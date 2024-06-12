// Copyright 2024 Bakhtiarov Alexander
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include "core/perf/include/perf.hpp"
#include "tbb/bakhtiarov_a_matrix_mult_tbb/include/ccs_matrix_mult.hpp"

using namespace std;

TEST(bakhtiarov_a_matrix_mult_tbb, test_pipeline_run) {
  // Create data
  size_t z = 500;
  size_t x = 500;
  size_t c = 500;
  std::vector<double> lhs_in(p * q);
  for (size_t i = 0; i < p; ++i) {
    if (i % 4 == 0)
      for (size_t j = 0; j < q; ++j) {
        lhs_in[i * q + j] = 1.0;
      }
  }
  std::vector<double> rhs_in(q * r);
  for (size_t i = 0; i < q; ++i) {
    for (size_t j = 0; j < r; ++j) {
      if (j % 5 == 0) rhs_in[i * r + j] = 1.0;
    }
  }
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTbb->inputs_count.emplace_back(p);
  taskDataTbb->inputs_count.emplace_back(q);
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTbb->inputs_count.emplace_back(q);
  taskDataTbb->inputs_count.emplace_back(r);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(p);
  taskDataTbb->outputs_count.emplace_back(r);

  // Create Task
  auto testTaskTbb = std::make_shared<SparseMatrixMultiTBBParallel>(taskDataTbb);

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

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < p; ++i) {
    for (size_t j = 0; j < r; ++j) {
      if (i % 4 == 0 && j % 5 == 0)
        EXPECT_DOUBLE_EQ(out[i * r + j], q);
      else
        EXPECT_DOUBLE_EQ(out[i * r + j], 0.0);
    }
  }
}

TEST(bakhtiarov_a_matrix_mult_tbb, test_task_run) {
  // Create data
  size_t p = 700;
  size_t q = 700;
  size_t r = 700;
  std::vector<double> lhs_in(p * q);
  for (size_t i = 0; i < p; ++i) {
    if (i % 4 == 0)
      for (size_t j = 0; j < q; ++j) {
        lhs_in[i * q + j] = 1.0;
      }
  }
  std::vector<double> rhs_in(q * r);
  for (size_t i = 0; i < q; ++i) {
    for (size_t j = 0; j < r; ++j) {
      if (j % 5 == 0) rhs_in[i * r + j] = 1.0;
    }
  }
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTbb->inputs_count.emplace_back(p);
  taskDataTbb->inputs_count.emplace_back(q);
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTbb->inputs_count.emplace_back(q);
  taskDataTbb->inputs_count.emplace_back(r);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(p);
  taskDataTbb->outputs_count.emplace_back(r);

  // Create Task
  auto testTaskTbb = std::make_shared<SparseMatrixMultiTBBParallel>(taskDataTbb);

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
  for (size_t i = 0; i < p; ++i) {
    for (size_t j = 0; j < r; ++j) {
      if (i % 4 == 0 && j % 5 == 0)
        EXPECT_DOUBLE_EQ(out[i * r + j], q);
      else
        EXPECT_DOUBLE_EQ(out[i * r + j], 0.0);
    }
  }
}
