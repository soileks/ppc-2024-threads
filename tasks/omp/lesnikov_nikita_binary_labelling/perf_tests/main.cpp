// Copyright 2024 Lesnikov Nikita
#include <gtest/gtest.h>
#include <omp.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/lesnikov_nikita_binary_labelling/include/ops_omp.hpp"

TEST(lesnikov_binary_labelling_perf_test, test_pipeline_run) {
  int m = 500;
  int n = 500;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);
  std::vector<uint8_t> outV_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(outV_omp.data());
  taskDataOmp->outputs_count.push_back(outV_omp.size());

  auto testTaskOmp = std::make_shared<BinaryLabellingOmp>(taskDataOmp);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(lesnikov_binary_labelling_perf_test, test_task_run) {
  int m = 500;
  int n = 500;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);
  std::vector<uint8_t> outV_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(outV_omp.data());
  taskDataOmp->outputs_count.push_back(outV_omp.size());

  auto testTaskOmp = std::make_shared<BinaryLabellingOmp>(taskDataOmp);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] { return omp_get_wtime(); };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskOmp);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
}
