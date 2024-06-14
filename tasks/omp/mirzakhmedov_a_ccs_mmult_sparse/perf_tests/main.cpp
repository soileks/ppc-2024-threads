// Copyright 2024 Mirzakhmedov Alexander
#include <gtest/gtest.h>
#include <omp.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/mirzakhmedov_a_ccs_mmult_sparse/include/ccs_mmult_sparse_omp.hpp"

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_pipeline_run) {
  MatrixInCCS_Sparse M1(2000000, 2000000);
  MatrixInCCS_Sparse M2(2000000, 1);
  MatrixInCCS_Sparse M3(2000000, 1);
  std::complex<double> vvector(2.0, 1.0);

  int l = 1;
  int m = 0;
  for (int i = 0; i <= 2000000; i++) {
    M1.col_p.push_back(m);
    m += l;
    l++;
  }

  l = 1;
  m = 0;
  for (int i = 0; i < M1.col_p[2000000]; i++) {
    M1.val.emplace_back(vvector);
    if (m >= l) {
      m = 0;
      l++;
    }
    M1.row.push_back(m);
    m++;
  }

  M2.col_p = {0, 2000000};
  for (int i = 0; i < 2000000; i++) {
    M2.val.emplace_back(vvector);
    M2.row.push_back(i);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  auto testTaskOmp = std::make_shared<Omp_MatMultCCS>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = omp_get_wtime();
  perfAttr->current_timer = [&] {
    auto current_time_point = omp_get_wtime();
    auto duration = current_time_point - t0;
    return duration;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_task_run) {
  MatrixInCCS_Sparse M1(2000000, 2000000);
  MatrixInCCS_Sparse M2(2000000, 1);
  MatrixInCCS_Sparse M3(2000000, 1);
  std::complex<double> vvector(2.0, 1.0);

  int l = 1;
  int m = 0;
  for (int i = 0; i <= 2000000; i++) {
    M1.col_p.emplace_back(m);
    m += l;
    l++;
  }

  l = 1;
  m = 0;
  for (int i = 0; i < M1.col_p[2000000]; i++) {
    M1.val.emplace_back(vvector);
    if (m >= l) {
      m = 0;
      l++;
    }
    M1.row.push_back(m);
    m++;
  }

  M2.col_p = {0, 2000000};
  for (int i = 0; i < 2000000; i++) {
    M2.val.emplace_back(vvector);
    M2.row.push_back(i);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  auto testTaskOmp = std::make_shared<Omp_MatMultCCS>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = omp_get_wtime();
  perfAttr->current_timer = [&] {
    auto current_time_point = omp_get_wtime();
    auto duration = current_time_point - t0;
    return duration;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}