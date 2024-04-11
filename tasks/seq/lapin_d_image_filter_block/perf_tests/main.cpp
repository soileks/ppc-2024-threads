// Copyright 2024 Lapin Dmitriy
#include <gtest/gtest.h>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/lapin_d_image_filter_block/include/ops_seq.hpp"

TEST(lapin_d_image_filter_block, test_pipeline_run_4000x4000) {
  // Create TaskData
  int height = 4000;
  int width = 4000;
  std::vector<std::vector<int>> in(height);
  for (int i = 0; i < height; i++) {
     in[i].resize(width);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        in[i][j] = rand() % 256;
    }
  }
  
  std::vector<std::vector<int>> out(height);
  for (int i = 0; i < height; i++) {
     out[i].resize(width);
  }
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));

  // Create Task
  auto testTaskSequential = std::make_shared<BlockFilterSeq>(taskDataSeq);

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
}

TEST(lapin_d_image_filter_block, test_task_run_run4000x4000) {
  // Create TaskData
    int height = 4000;
  int width = 4000;
  std::vector<std::vector<int>> in(height);
  for (int i = 0; i < height; i++) {
     in[i].resize(width);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        in[i][j] = rand() % 256;
    }
  }
  
  std::vector<std::vector<int>> out(height);
  for (int i = 0; i < height; i++) {
     out[i].resize(width);
  }
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&in));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(&out));

  // Create Task
  auto testTaskSequential = std::make_shared<BlockFilterSeq>(taskDataSeq);

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
}