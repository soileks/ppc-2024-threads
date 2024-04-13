// Copyright 2024 Vetoshnikova Ekaterina
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/vetoshnikova_k_convex_hull_bin_image_components/include/ops_seq.hpp"

TEST(vetoshnikova_k_hull_bin_image_seq, test_pipeline_run) {
  int h = 10000;
  int w = 7;
  // Create data
  std::vector<int> out(200);
  std::vector<uint8_t> in(h * w, 0);
  in[0] = 1;
  in[1] = 1;
  in[7] = 1;
  in[8] = 1;
  std::vector<int> hullTrue = {0, 0, 0, 1, 1, 1, 1, 0, -1};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
  taskDataSeq->inputs_count.emplace_back(h);
  taskDataSeq->inputs_count.emplace_back(w);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));

  // Create Task
  auto testTaskSequential = std::make_shared<ConstructingConvexHullSeq>(taskDataSeq);

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
  for (size_t i = 0; i < hullTrue.size(); i++) {
    ASSERT_EQ(hullTrue[i], out[i]);
  }
}

TEST(vetoshnikova_k_hull_bin_image_seq, test_task_run) {
  int h = 10000;
  int w = 7;
  // Create data
  std::vector<int> out(200);
  std::vector<uint8_t> in(h * w, 0);
  in[0] = 1;
  in[1] = 1;
  in[7] = 1;
  in[8] = 1;
  std::vector<int> hullTrue = {0, 0, 0, 1, 1, 1, 1, 0, -1};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
  taskDataSeq->inputs_count.emplace_back(h);
  taskDataSeq->inputs_count.emplace_back(w);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));

  // Create Task
  auto testTaskSequential = std::make_shared<ConstructingConvexHullSeq>(taskDataSeq);

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
  for (size_t i = 0; i < hullTrue.size(); i++) {
    ASSERT_EQ(hullTrue[i], out[i]);
  }
}
