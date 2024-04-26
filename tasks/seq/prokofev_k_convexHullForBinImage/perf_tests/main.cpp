// Copyright 2024 Prokofev Kirill
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/prokofev_k_convexHullForBinImage/include/ops_seq.hpp"

TEST(prokofev_k_convex_hull_seq, test_pipeline_run) {
  // Create data
  int width = 8;
  int height = 500000;
  std::vector<int> out(9);
  std::vector<int> row = {0, 0, 1, 1, 1, 1, 0, 0};
  std::vector<std::vector<int>> notVecImage(height, row);
  std::vector<int> vecImage = prokofev_k_covexHull_Seq::ConvertImageToVector(notVecImage, width, height);
  std::vector<int> trueRes = {2, 0, 5, 0, 5, 499999, 2, 499999, -1};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(out.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  auto testTaskSequential = std::make_shared<prokofev_k_covexHull_Seq::BinaryImageConvexHullSeq>(taskDataSeq);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < trueRes.size(); i++) {
    ASSERT_EQ(trueRes[i], out[i]);
  }
}

TEST(prokofev_k_convex_hull_seq, test_task_run) {
  // Create data
  int width = 8;
  int height = 500000;
  std::vector<int> out(9);
  std::vector<int> row = {0, 0, 1, 1, 1, 1, 0, 0};
  std::vector<std::vector<int>> notVecImage(height, row);
  std::vector<int> vecImage = prokofev_k_covexHull_Seq::ConvertImageToVector(notVecImage, width, height);
  std::vector<int> trueRes = {2, 0, 5, 0, 5, 499999, 2, 499999, -1};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(out.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  auto testTaskSequential = std::make_shared<prokofev_k_covexHull_Seq::BinaryImageConvexHullSeq>(taskDataSeq);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < trueRes.size(); i++) {
    ASSERT_EQ(trueRes[i], out[i]);
  }
}