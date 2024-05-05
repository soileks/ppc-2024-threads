// Copyright 2024 Khodyrev Fedor
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <stack>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/khodyrev_f_convex_hull/include/ops_tbb.hpp"

using namespace khodyrev_tbb;

TEST(khodyrev_f_convex_null_tbb, test_pipeline_run) {
  int height_in = 13000;
  int width_in = 1000;
  int height_out = 13000;
  int width_out = 1000;
  std::vector<int> in(13000000, 0);
  in[0] = 1;
  in[1] = 1;
  std::vector<int> out(height_out * width_out, 0);

  std::vector<int> true_result(13000000, 0);
  true_result[0] = 1;
  true_result[1] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();

  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(height_in);
  taskDataTbb->inputs_count.emplace_back(width_in);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(height_out);
  taskDataTbb->outputs_count.emplace_back(width_out);

  auto testTaskTbb = std::make_shared<KhodyrevTaskTBB>(taskDataTbb);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    ASSERT_EQ(out[i], true_result[i]);
  }
}

TEST(khodyrev_f_convex_null_tbb, test_task_run) {
  int height_in = 13000;
  int width_in = 1000;
  int height_out = 13000;
  int width_out = 1000;
  std::vector<int> in(13000000, 0);
  in[0] = 1;
  in[1] = 1;
  std::vector<int> out(height_out * width_out, 0);

  std::vector<int> true_result(13000000, 0);
  true_result[0] = 1;
  true_result[1] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();

  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTbb->inputs_count.emplace_back(height_in);
  taskDataTbb->inputs_count.emplace_back(width_in);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTbb->outputs_count.emplace_back(height_out);
  taskDataTbb->outputs_count.emplace_back(width_out);

  auto testTaskTbb = std::make_shared<KhodyrevTaskTBB>(taskDataTbb);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    ASSERT_EQ(out[i], true_result[i]);
  }
}