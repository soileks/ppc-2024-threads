// Copyright 2024 Shubin Mikhail

#include <gtest/gtest.h>
#include <tbb/tbb.h>

#include "core/perf/include/perf.hpp"
#include "tbb/shubin_m_double_crs_mult/include/ops_tbb.hpp"

TEST(shubin_m_double_crs_mult_tbb, test_pipeline_run) {
  size_t mat_in_1_row = 700;
  size_t mat_in_cr = 700;
  size_t mat_in_2_col = 700;
  std::vector<double> A(mat_in_1_row * mat_in_cr);
  for (size_t i = 0; i < mat_in_1_row; i++) {
    if (i % 2 == 0) {
      for (size_t j = 0; j < mat_in_cr; j++) {
        A[i * mat_in_cr + j] = 1.0;
      }
    }
  }
  std::vector<double> B(mat_in_cr * mat_in_2_col);
  for (size_t i = 0; i < mat_in_cr; i++) {
    for (size_t j = 0; j < mat_in_2_col; j++) {
      if (j % 2 == 0) {
        B[i * mat_in_2_col + j] = 1.0;
      }
    }
  }

  SparseMat_CRS mat_in_1(A, mat_in_1_row, mat_in_cr);
  SparseMat_CRS mat_in_2(B, mat_in_cr, mat_in_2_col);
  SparseMat_CRS mat_out(mat_in_1_row, mat_in_2_col);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&mat_in_1));
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&mat_in_2));
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(&mat_out));

  auto testTaskTBB = std::make_shared<SparseMat_CRS_Mult_TBB>(taskDataTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::vector<double> res = CRS_to_vector(mat_out);

  for (size_t i = 0; i < mat_in_1_row; i++) {
    for (size_t j = 0; j < mat_in_2_col; j++) {
      if (i % 2 == 0 && j % 2 == 0)
        EXPECT_DOUBLE_EQ(res[i * mat_in_2_col + j], mat_in_cr);
      else
        EXPECT_DOUBLE_EQ(res[i * mat_in_2_col + j], 0.0);
    }
  }
}

TEST(shubin_m_double_crs_mult_tbb, test_task_run) {
  size_t mat_in_1_row = 700;
  size_t mat_in_cr = 700;
  size_t mat_in_2_col = 700;
  std::vector<double> A(mat_in_1_row * mat_in_cr);
  for (size_t i = 0; i < mat_in_1_row; i++) {
    if (i % 2 == 0) {
      for (size_t j = 0; j < mat_in_cr; j++) {
        A[i * mat_in_cr + j] = 1.0;
      }
    }
  }
  std::vector<double> B(mat_in_cr * mat_in_2_col);
  for (size_t i = 0; i < mat_in_cr; i++) {
    for (size_t j = 0; j < mat_in_2_col; j++) {
      if (j % 2 == 0) {
        B[i * mat_in_2_col + j] = 1.0;
      }
    }
  }

  SparseMat_CRS mat_in_1(A, mat_in_1_row, mat_in_cr);
  SparseMat_CRS mat_in_2(B, mat_in_cr, mat_in_2_col);
  SparseMat_CRS mat_out(mat_in_1_row, mat_in_2_col);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&mat_in_1));
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(&mat_in_2));
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(&mat_out));

  auto testTaskTBB = std::make_shared<SparseMat_CRS_Mult_TBB>(taskDataTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  std::vector<double> res = CRS_to_vector(mat_out);

  for (size_t i = 0; i < mat_in_1_row; i++) {
    for (size_t j = 0; j < mat_in_2_col; j++) {
      if (i % 2 == 0 && j % 2 == 0)
        EXPECT_DOUBLE_EQ(res[i * mat_in_2_col + j], mat_in_cr);
      else
        EXPECT_DOUBLE_EQ(res[i * mat_in_2_col + j], 0.0);
    }
  }
}
