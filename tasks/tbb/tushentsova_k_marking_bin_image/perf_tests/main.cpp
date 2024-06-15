// Copyright 2024 Tushentsova Karina
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/tushentsova_k_marking_bin_image/include/ops_tbb.hpp"

TEST(tushentsova_k_marking_bin_image_perf_test, test_pipeline_run) {
  uint32_t height = 1100;
  uint32_t width = 1100;

  std::vector<uint32_t> size = {height, width};
  std::vector<uint8_t> in(height * width, 0);
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components(height * width, 1);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataPar->inputs_count.emplace_back(size.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  auto testTaskPar = std::make_shared<markingImageTBB>(taskDataPar);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskPar);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  ASSERT_EQ(out, components);
}

TEST(tushentsova_k_marking_bin_image_perf_test, test_task_run) {
  uint32_t height = 1200;
  uint32_t width = 1200;

  std::vector<uint32_t> size = {height, width};
  std::vector<uint8_t> in(height * width, 0);
  std::vector<uint8_t> out(height * width, 0);
  std::vector<uint8_t> components(height * width, 1);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataPar->inputs_count.emplace_back(size.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataPar->outputs_count.emplace_back(out.size());

  auto testTaskPar = std::make_shared<markingImageTBB>(taskDataPar);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskPar);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  ASSERT_EQ(out, components);
}