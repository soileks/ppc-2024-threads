// Copyright 2024 Safarov Nurlan
#include <gtest/gtest.h>

#include <chrono>
#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/safarov_n_sparse_matmult_crs/include/sparse_matmult_crs.hpp"

SparseMatrixCRS getRandomMatrixCRS(int _numberOfRows, int _numberOfColumns, double chance, int seed) {
  std::mt19937 gen;
  gen.seed(seed);
  std::uniform_real_distribution<double> rnd(-5.0, 5.0);
  std::bernoulli_distribution bd(chance);

  SparseMatrixCRS randomMatrix;

  randomMatrix.numberOfRows = _numberOfRows;
  randomMatrix.numberOfColumns = _numberOfColumns;
  randomMatrix.pointers.assign(randomMatrix.numberOfRows + 1, 0);

  std::vector<std::vector<std::pair<int, double>>> temp(randomMatrix.numberOfRows);

  for (int i = 0; i < _numberOfRows; i++) {
    for (int j = 0; j < _numberOfColumns; j++) {
      if (bd(gen)) {
        double v = rnd(gen);
        temp[i].emplace_back(j, v);
      }
    }
  }

  for (int i = 0; i < randomMatrix.numberOfRows; i++) {
    randomMatrix.pointers[i + 1] = randomMatrix.pointers[i];
    for (auto &t : temp[i]) {
      randomMatrix.columnIndexes.push_back(t.first);
      randomMatrix.values.push_back(t.second);
      randomMatrix.pointers[i + 1]++;
    }
  }

  return randomMatrix;
}

TEST(Safarov_N_SparseMatMultCRS, test_pipeline_run) {
  SparseMatrixCRS X = getRandomMatrixCRS(105, 105, 0.80, 2200);
  SparseMatrixCRS Y = getRandomMatrixCRS(105, 105, 0.80, 3150);
  SparseMatrixCRS Z;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&X));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&Y));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Z));

  // Create Task
  auto taskSequential = std::make_shared<SparseMatrixMultiplicationCRS>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(current_time_point - t0).count();
    return duration;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(Safarov_N_SparseMatMultCRS, test_task_run) {
  SparseMatrixCRS X = getRandomMatrixCRS(105, 105, 0.80, 2200);
  SparseMatrixCRS Y = getRandomMatrixCRS(105, 105, 0.80, 3150);
  SparseMatrixCRS Z;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&X));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&Y));
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&Z));

  // Create Task
  auto taskSequential = std::make_shared<SparseMatrixMultiplicationCRS>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(current_time_point - t0).count();
    return duration;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
