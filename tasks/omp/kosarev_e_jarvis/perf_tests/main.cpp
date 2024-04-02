// Copyright 2024 Kosarev Egor
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/kosarev_e_jarvis/include/ops_omp.hpp"

TEST(kosarev_e_jarvis_hull_omp, test_pipeline_run) {
  std::vector<Point> points = {{1, 4}, {1, 5}, {1, 6}, {1, 2}, {1, 3}, {2, 5}, {3, 4}, {0, 5}, {1, 7}};
  std::vector<Point> hull = {{1, 2}, {0, 5}, {1, 7}, {3, 4}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  auto testTaskSequential = std::make_shared<TestOMPTaskParallelKosarevJarvisHull>(taskDataSeq);

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
  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(resHull[i], hull[i]);
  }
}

TEST(kosarev_e_jarvis_hull_omp, test_task_run) {
  std::vector<Point> points = {{1, 4}, {1, 5}, {1, 6}, {1, 2}, {1, 3}, {2, 5}, {3, 4}, {0, 5}, {1, 7},
                               {1, 4}, {1, 5}, {1, 6}, {1, 2}, {1, 3}, {2, 5}, {3, 4}, {0, 5}, {1, 7},
                               {1, 4}, {1, 5}, {1, 6}, {1, 2}, {1, 3}, {2, 5}, {3, 4}, {0, 5}, {1, 7},
                               {1, 4}, {1, 5}, {1, 6}, {1, 2}, {1, 3}, {2, 5}, {3, 4}, {0, 5}, {1, 7}};
  std::vector<Point> hull = {{1, 2}, {0, 5}, {1, 7}, {3, 4}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  auto testTaskSequential = std::make_shared<TestOMPTaskParallelKosarevJarvisHull>(taskDataSeq);

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
  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(resHull[i], hull[i]);
  }
}