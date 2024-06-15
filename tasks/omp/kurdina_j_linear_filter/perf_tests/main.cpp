// Copyright 2024 Kurdina Julia
#include <gtest/gtest.h>
#include <omp.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/kurdina_j_linear_filter/include/ops_omp.hpp"

TEST(kurdina_j_linear_filter_omp, test_pipeline_run) {
  int n = 2000;
  int m = 2000;
  std::vector<uint8_t> gauss_kernel = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  // Create data
  std::vector<uint8_t> in(n * m, 255);
  std::vector<int> size_m = {n, m};
  std::vector<uint8_t> out_parallel(n * m);
  std::vector<uint8_t> result(n * m, 255);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataPar->inputs_count.emplace_back(size_m.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataPar->inputs_count.emplace_back(gauss_kernel.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_parallel.data()));
  taskDataPar->outputs_count.emplace_back(out_parallel.size());

  // Create Task
  auto testTaskOMP = std::make_shared<HorizontalSplitOMP_parallel>(taskDataPar);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(result[i], out_parallel[i]);
  }
}

TEST(kurdina_j_linear_filter_omp, test_task_run) {
  int n = 2000;
  int m = 2000;
  std::vector<uint8_t> gauss_kernel = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  // Create data
  std::vector<uint8_t> in(n * m, 255);
  std::vector<int> size_m = {n, m};
  std::vector<uint8_t> out_parallel(n * m, 0);
  std::vector<uint8_t> result(n * m, 255);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataPar->inputs_count.emplace_back(size_m.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataPar->inputs_count.emplace_back(gauss_kernel.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_parallel.data()));
  taskDataPar->outputs_count.emplace_back(out_parallel.size());

  // Create Task
  auto testTaskOMP = std::make_shared<HorizontalSplitOMP_parallel>(taskDataPar);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(result[i], out_parallel[i]);
  }
}
