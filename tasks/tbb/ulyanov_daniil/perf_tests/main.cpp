// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/ulyanov_daniil/include/ops_tbb.hpp"

TEST(tbb_example_perf_test_ulyanov, test_pipeline_run_ulyanov) {
  int height = 500;
  int width = 500;

  // Create data
  std::vector<uint8_t> in(height * width * 3);
  for (size_t i = 0; i < in.size(); i++) {
    in[i] = i % 255;
  }
  std::vector<uint8_t> out_seq(height * width * 3);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(in.data());
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(out_seq.data());
  taskDataSeq->outputs_count.emplace_back(height);
  taskDataSeq->outputs_count.emplace_back(width);

  // Create Task
  auto testTaskTBB = std::make_shared<TBBFilterGaussHorizontalSeqUlyanov>(taskDataSeq);

  // Create data
  std::vector<uint8_t> out_par(height * width * 3);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(in.data());
  taskDataPar->inputs_count.emplace_back(height);
  taskDataPar->inputs_count.emplace_back(width);
  taskDataPar->outputs.emplace_back(out_par.data());
  taskDataPar->outputs_count.emplace_back(height);
  taskDataPar->outputs_count.emplace_back(width);

  // Create Task
  TBBFilterGaussHorizontalParUlyanov testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(out_seq, out_par);
}

TEST(tbb_example_perf_test_ulyanov, test_task_run_ulyanov) {
  int height = 500;
  int width = 500;

  // Create data
  std::vector<uint8_t> in(height * width * 3);
  for (size_t i = 0; i < in.size(); i++) {
    in[i] = i % 255;
  }
  std::vector<uint8_t> out_seq(height * width * 3);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(in.data());
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(out_seq.data());
  taskDataSeq->outputs_count.emplace_back(height);
  taskDataSeq->outputs_count.emplace_back(width);

  // Create Task
  auto testTaskTBB = std::make_shared<TBBFilterGaussHorizontalSeqUlyanov>(taskDataSeq);

  // Create data
  std::vector<uint8_t> out_par(height * width * 3);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(in.data());
  taskDataPar->inputs_count.emplace_back(height);
  taskDataPar->inputs_count.emplace_back(width);
  taskDataPar->outputs.emplace_back(out_par.data());
  taskDataPar->outputs_count.emplace_back(height);
  taskDataPar->outputs_count.emplace_back(width);

  // Create Task
  TBBFilterGaussHorizontalParUlyanov testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskTBB);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(out_seq, out_par);
}