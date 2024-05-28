// Copyright 2024 Bodrov Daniil
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <chrono>
#include <complex>
#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/bodrov_d_crs_matr/include/bodrov_d_crs_matr_tbb.hpp"

using namespace bodrov_tbb;

SparseMatrixBodrovOMP generate_random_matrix1(int n, int m, double proba, int seed) {
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> random(-2.0, 2.0);
  std::bernoulli_distribution bernoulli(proba);

  SparseMatrixBodrovOMP result;
  result.Rows = n;
  result.Columns = m;
  result.DataPointer.assign(result.Rows + 1, 0);

  std::vector<int> ColumnsIndexes;
  std::vector<std::complex<double>> Values;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (bernoulli(gen)) {
        ColumnsIndexes.push_back(j);
        Values.emplace_back(random(gen), random(gen));
      }
    }
    result.DataPointer[i + 1] = ColumnsIndexes.size();
  }

  result.ColumnsIndexes = std::move(ColumnsIndexes);
  result.Values = std::move(Values);

  return result;
}

TEST(bodrov_d_crs_matr_tbb, test_pipeline_run) {
  SparseMatrixBodrovOMP A = generate_random_matrix1(100, 100, 0.6, 4113);
  SparseMatrixBodrovOMP B = generate_random_matrix1(100, 100, 0.6, 2134);
  SparseMatrixBodrovOMP Result;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result));

  // Create Task
  auto taskTBB = std::make_shared<SparseMatrixSolverBodrovOMPParallel>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 50;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(bodrov_d_crs_matr_tbb, test_task_run) {
  SparseMatrixBodrovOMP A = generate_random_matrix1(100, 100, 0.6, 4113);
  SparseMatrixBodrovOMP B = generate_random_matrix1(100, 100, 0.6, 2134);
  SparseMatrixBodrovOMP Result;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&A));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&B));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Result));

  // Create Task
  auto taskTBB = std::make_shared<SparseMatrixSolverBodrovOMPParallel>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 50;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
