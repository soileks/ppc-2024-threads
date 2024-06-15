// Copyright 2024 Kokin Ivan
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/kokin_i_image_marking_tbb/include/ops_tbb.hpp"

TEST(kokin_marking_component_bin_image_test_tbb, test_run) {
  uint32_t ht = 1200;
  uint32_t wh = 1200;
  std::vector<uint32_t> size = {ht, wh};
  std::vector<uint8_t> in(ht * wh, 0);
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt(ht * wh, 1);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataTBB->inputs_count.emplace_back(size.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  auto taskTestTBB = std::make_shared<imageMarkingTBB>(taskDataTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 100;
  const auto tm = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - tm).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskTestTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  ASSERT_EQ(out, cmpt);
}

TEST(kokin_marking_component_bin_image_test_tbb, test_task_run) {
  uint32_t ht = 1200;
  uint32_t wh = 1200;
  std::vector<uint32_t> size = {ht, wh};
  std::vector<uint8_t> in(ht * wh, 0);
  std::vector<uint8_t> out(ht * wh, 0);
  std::vector<uint8_t> cmpt(ht * wh, 1);

  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(size.data()));
  taskDataTBB->inputs_count.emplace_back(size.size());
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataTBB->inputs_count.emplace_back(in.size());
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(out.size());

  auto taskTestTBB = std::make_shared<imageMarkingTBB>(taskDataTBB);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto tm = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - tm).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(taskTestTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(out, cmpt);
}
