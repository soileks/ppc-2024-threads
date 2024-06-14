// Copyright 2024 Veselov Ilya
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/veselov_i_systemsgradmethod/include/systemsgradmethod_tbb.hpp"

using namespace veselov_i_tbb;

TEST(veselov_i_systems_grad_method_tbb, test_pipeline) {
  int rows = 200;

  std::vector<double> matrix = genRandomMatrix(rows, 10);
  std::vector<double> vec = genRandomVector(rows, 10);
  std::vector<double> res(rows);

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix.data()));
  taskDataTbb->inputs_count.emplace_back(matrix.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataTbb->inputs_count.emplace_back(vec.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&rows));
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataTbb->outputs_count.emplace_back(vec.size());

  auto testTaskTbb = std::make_shared<SystemsGradMethodTbb>(taskDataTbb);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(checkSolution(matrix, vec, res, 1e-6));
}

TEST(veselov_i_systems_grad_method_tbb, test_task_run) {
  int rows = 200;

  std::vector<double> matrix = genRandomMatrix(rows, 10);
  std::vector<double> vec = genRandomVector(rows, 10);
  std::vector<double> res(rows);

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix.data()));
  taskDataTbb->inputs_count.emplace_back(matrix.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataTbb->inputs_count.emplace_back(vec.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&rows));
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataTbb->outputs_count.emplace_back(vec.size());

  auto testTaskTbb = std::make_shared<SystemsGradMethodTbb>(taskDataTbb);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_TRUE(checkSolution(matrix, vec, res, 1e-6));
}
