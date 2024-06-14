// Copyright 2024 Saratova Marina
#include <gtest/gtest.h>
#include <omp.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/saratova_m_mult_matrix_fox/include/ops_seq.hpp"

using namespace saratova_omp;

TEST(Saratova_M_Mult_Matrix_Fox, test_pipeline_run) {
  double scaleFactor = 43.0;
  size_t matrixSize = 580;
  std::vector<double> matrixA(matrixSize * matrixSize);
  std::vector<double> matrixB(matrixSize * matrixSize);
  std::vector<double> resultMatrix(matrixSize * matrixSize);
  FillRandomValues(matrixA.data(), matrixA.size());
  GenerateIdentityMatrix(matrixB.data(), matrixSize, scaleFactor);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(matrixA.data()));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(matrixB.data()));
  taskDataSeq->inputs_count.emplace_back(matrixA.size());
  taskDataSeq->inputs_count.emplace_back(matrixB.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(resultMatrix.data()));
  taskDataSeq->outputs_count.emplace_back(resultMatrix.size());

  // Create Task
  auto saratovaTaskOmp = std::make_shared<SaratovaTaskOmp>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(saratovaTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < matrixSize * matrixSize; i++) {
    EXPECT_DOUBLE_EQ(resultMatrix[i], scaleFactor * matrixA[i]);
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, test_task_run) {
  double scaleFactor = 43.0;
  size_t matrixSize = 580;
  std::vector<double> matrixA(matrixSize * matrixSize);
  std::vector<double> matrixB(matrixSize * matrixSize);
  std::vector<double> resultMatrix(matrixSize * matrixSize);
  FillRandomValues(matrixA.data(), matrixA.size());
  GenerateIdentityMatrix(matrixB.data(), matrixSize, scaleFactor);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(matrixA.data()));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(matrixB.data()));
  taskDataSeq->inputs_count.emplace_back(matrixA.size());
  taskDataSeq->inputs_count.emplace_back(matrixB.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(resultMatrix.data()));
  taskDataSeq->outputs_count.emplace_back(resultMatrix.size());

  // Create Task
  auto saratovaTaskOmp = std::make_shared<SaratovaTaskOmp>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(saratovaTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < matrixSize * matrixSize; i++) {
    EXPECT_DOUBLE_EQ(resultMatrix[i], scaleFactor * matrixA[i]);
  }
}
