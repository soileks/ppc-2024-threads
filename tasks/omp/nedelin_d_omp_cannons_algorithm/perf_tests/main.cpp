// Copyright 2024 Nedelin Dmitry
#include <gtest/gtest.h>
#include <omp.h>

#include "core/perf/include/perf.hpp"
#include "omp/nedelin_d_omp_cannons_algorithm/include/ops_omp.hpp"

TEST(openmp_nedelin_d_block_cannons_perf_test, test_pipeline_run) {
  int n = 500;
  int m = 500;

  std::vector<double> in_mtrx_A = RndMatrix(n, m);
  std::vector<double> in_mtrx_B = RndMatrix(n, m);
  std::vector<double> out(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  std::vector<double> res = multiplyMtrx(in_mtrx_A, in_mtrx_B, n, m);

  auto testTaskOMP = std::make_shared<TestTaskOMPParallelNedelinCannon>(taskDataSeq);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(res[i], out[i]);
  }
}

TEST(openmp_nedelin_d_block_cannons_perf_test, test_task_run) {
  int n = 500;
  int m = 500;

  std::vector<double> in_mtrx_A = RndMatrix(n, m);
  std::vector<double> in_mtrx_B = RndMatrix(n, m);
  std::vector<double> out(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  std::vector<double> res = multiplyMtrx(in_mtrx_A, in_mtrx_B, n, m);

  auto testTaskOMP = std::make_shared<TestTaskOMPParallelNedelinCannon>(taskDataSeq);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOMP);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(res[i], out[i]);
  }
}