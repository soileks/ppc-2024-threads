// Copyright 2024 Borovkov Sergey
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/borovkov_s_can_stl/include/ops_stl.hpp"

TEST(borovkov_s_stl_perf_tests, test_900x900) {
  // Create data
  size_t size = 900;
  size_t block = 450;

  double minVal = -100.0;
  double maxVal = 100.0;

  auto inputMatrOne = BorovkovStl::getRandomSquareMatrix(size, minVal, maxVal);
  auto inputMatrTwo = BorovkovStl::getRandomSquareMatrix(size, minVal, maxVal);
  std::vector<double> outputMatr(size * size, 0.0);

  // Create TaskData
  auto taskDataStl = std::make_shared<ppc::core::TaskData>();

  // Add matrOne
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrOne.data()));
  taskDataStl->inputs_count.emplace_back(inputMatrOne.size());

  // Add matrTwo
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrTwo.data()));
  taskDataStl->inputs_count.emplace_back(inputMatrTwo.size());

  // Add size
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));

  // Add block
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(&block));

  // Add out matr
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputMatr.data()));
  taskDataStl->outputs_count.emplace_back(outputMatr.size());

  auto resSeq = BorovkovStl::CannonMatrixMultSeq(inputMatrOne, inputMatrTwo, size, block);

  // Create Task
  auto testTaskStl = std::make_shared<BorovkovStl::BorovkovCanMultMatrStl>(taskDataStl);

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

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskStl);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  auto resSize = resSeq.size();

  for (size_t i = 0; i < resSize; ++i) {
    ASSERT_TRUE(BorovkovStl::isEqual(resSeq[i], outputMatr[i]));
  }
}

TEST(borovkov_s_stl_perf_tests, test_800x800) {
  // Create data
  size_t size = 800;
  size_t block = 400;

  double minVal = -100.0;
  double maxVal = 100.0;

  auto inputMatrOne = BorovkovStl::getRandomSquareMatrix(size, minVal, maxVal);
  auto inputMatrTwo = BorovkovStl::getRandomSquareMatrix(size, minVal, maxVal);
  std::vector<double> outputMatr(size * size, 0.0);

  // Create TaskData
  auto taskDataStl = std::make_shared<ppc::core::TaskData>();

  // Add matrOne
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrOne.data()));
  taskDataStl->inputs_count.emplace_back(inputMatrOne.size());

  // Add matrTwo
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(inputMatrTwo.data()));
  taskDataStl->inputs_count.emplace_back(inputMatrTwo.size());

  // Add size
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(&size));

  // Add block
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(&block));

  // Add out matr
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(outputMatr.data()));
  taskDataStl->outputs_count.emplace_back(outputMatr.size());

  auto resSeq = BorovkovStl::CannonMatrixMultSeq(inputMatrOne, inputMatrTwo, size, block);

  // Create Task
  auto testTaskStl = std::make_shared<BorovkovStl::BorovkovCanMultMatrStl>(taskDataStl);

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

  // Create Perf analyzer.
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskStl);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  auto resSize = resSeq.size();

  for (size_t i = 0; i < resSize; ++i) {
    ASSERT_TRUE(BorovkovStl::isEqual(resSeq[i], outputMatr[i]));
  }
}
