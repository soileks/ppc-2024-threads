// Copyright 2024 Borovkov Sergey
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/borovkov_s_can_tbb/include/ops_tbb.hpp"

TEST(borovkov_s_tbb_perf_tests, test_900x900) {
  // Create data
  size_t size = 900;
  size_t block = 450;

  double minVal = -100.0;
  double maxVal = 100.0;

  auto inputMatrOne = BorovkovTbb::getRandomSquareMatrix(size, minVal, maxVal);
  auto inputMatrTwo = BorovkovTbb::getRandomSquareMatrix(size, minVal, maxVal);
  std::vector<double> outputMatr(size * size, 0.0);

  // Create TaskData
  auto taskDataTbb = std::make_shared<ppc::core::TaskData>();

  // Add matrOne
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrOne.data()));
  taskDataTbb->inputs_count.emplace_back(inputMatrOne.size());

  // Add matrTwo
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrTwo.data()));
  taskDataTbb->inputs_count.emplace_back(inputMatrTwo.size());

  // Add size
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));

  // Add block
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&block));

  // Add out matr
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputMatr.data()));
  taskDataTbb->outputs_count.emplace_back(outputMatr.size());

  auto resSeq = BorovkovTbb::CannonMatrixMultSeq(inputMatrOne, inputMatrTwo, size, block);

  // Create Task
  auto testTaskTbb = std::make_shared<BorovkovTbb::BorovkovCanMultMatrTbb>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  auto resSize = resSeq.size();

  for (size_t i = 0; i < resSize; ++i) {
    ASSERT_TRUE(BorovkovTbb::isEqual(resSeq[i], outputMatr[i]));
  }
}

TEST(borovkov_s_tbb_perf_tests, test_800x800) {
  // Create data
  size_t size = 800;
  size_t block = 400;

  double minVal = -100.0;
  double maxVal = 100.0;

  auto inputMatrOne = BorovkovTbb::getRandomSquareMatrix(size, minVal, maxVal);
  auto inputMatrTwo = BorovkovTbb::getRandomSquareMatrix(size, minVal, maxVal);
  std::vector<double> outputMatr(size * size, 0.0);

  // Create TaskData
  auto taskDataTbb = std::make_shared<ppc::core::TaskData>();

  // Add matrOne
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrOne.data()));
  taskDataTbb->inputs_count.emplace_back(inputMatrOne.size());

  // Add matrTwo
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrTwo.data()));
  taskDataTbb->inputs_count.emplace_back(inputMatrTwo.size());

  // Add size
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));

  // Add block
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&block));

  // Add out matr
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputMatr.data()));
  taskDataTbb->outputs_count.emplace_back(outputMatr.size());

  auto resSeq = BorovkovTbb::CannonMatrixMultSeq(inputMatrOne, inputMatrTwo, size, block);

  // Create Task
  auto testTaskTbb = std::make_shared<BorovkovTbb::BorovkovCanMultMatrTbb>(taskDataTbb);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer.
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  auto resSize = resSeq.size();

  for (size_t i = 0; i < resSize; ++i) {
    ASSERT_TRUE(BorovkovTbb::isEqual(resSeq[i], outputMatr[i]));
  }
}
