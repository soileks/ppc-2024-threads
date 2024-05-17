// Copyright 2024 Lesnikov Nikita
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/lesnikov_nikita_binary_labelling/include/ops_tbb.hpp"

TEST(lesnikov_binary_labelling_perf_test_tbb, test_pipeline_run) {
  int m = 500;
  int n = 500;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);
  std::vector<uint8_t> outV_Tbb(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataTbb->inputs.push_back(in.data());
  taskDataTbb->inputs.push_back(serializedM.data());
  taskDataTbb->inputs.push_back(serializedN.data());
  taskDataTbb->inputs_count.push_back(in.size());
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->outputs.push_back(outV_Tbb.data());
  taskDataTbb->outputs_count.push_back(outV_Tbb.size());

  auto testTaskTbb = std::make_shared<BinaryLabellingTbb>(taskDataTbb);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(lesnikov_binary_labelling_perf_test_tbb, test_task_run) {
  int m = 500;
  int n = 500;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);
  std::vector<uint8_t> outV_Tbb(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataTbb->inputs.push_back(in.data());
  taskDataTbb->inputs.push_back(serializedM.data());
  taskDataTbb->inputs.push_back(serializedN.data());
  taskDataTbb->inputs_count.push_back(in.size());
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->inputs_count.push_back(4);
  taskDataTbb->outputs.push_back(outV_Tbb.data());
  taskDataTbb->outputs_count.push_back(outV_Tbb.size());

  auto testTaskTbb = std::make_shared<BinaryLabellingTbb>(taskDataTbb);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTbb);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}