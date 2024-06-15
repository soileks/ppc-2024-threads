// Copyright 2024 Lesnikov Nikita
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/lesnikov_binary_labelling_thread/include/ops_stl.hpp"

TEST(lesnikov_binary_labelling_perf_test, test_pipeline_run) {
  int m = 500;
  int n = 500;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);
  std::vector<uint8_t> outV_Thread(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataThread = std::make_shared<ppc::core::TaskData>();
  taskDataThread->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataThread->inputs.push_back(in.data());
  taskDataThread->inputs.push_back(serializedM.data());
  taskDataThread->inputs.push_back(serializedN.data());
  taskDataThread->inputs_count.push_back(in.size());
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->outputs.push_back(outV_Thread.data());
  taskDataThread->outputs_count.push_back(outV_Thread.size());

  auto testTaskThread = std::make_shared<BinaryLabellingThread>(taskDataThread);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskThread);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(lesnikov_binary_labelling_perf_test, test_task_run) {
  int m = 500;
  int n = 500;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVectorForLab(m * n);
  std::vector<uint8_t> outV_Thread(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataThread = std::make_shared<ppc::core::TaskData>();
  taskDataThread->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataThread->inputs.push_back(in.data());
  taskDataThread->inputs.push_back(serializedM.data());
  taskDataThread->inputs.push_back(serializedN.data());
  taskDataThread->inputs_count.push_back(in.size());
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->inputs_count.push_back(4);
  taskDataThread->outputs.push_back(outV_Thread.data());
  taskDataThread->outputs_count.push_back(outV_Thread.size());

  auto testTaskThread = std::make_shared<BinaryLabellingThread>(taskDataThread);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskThread);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
