// Copyright 2024 Filatov Maxim
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/filatov_m_linear_image_filtering_tbb/include/ops_tbb.hpp"

using namespace filatov_tbb;

class filatov_m_linear_image_filtering_perf_tbb : public ::testing::Test {
 protected:
  std::shared_ptr<ppc::core::TaskData> taskData;
  std::vector<uint8_t> inputData;
  std::vector<uint8_t> outputData;
  std::vector<uint8_t> expectedData;
  uint64_t width;
  uint64_t height;
  std::shared_ptr<GaussFilterHorizontal> gaussFilterHorizontal;

  // Передайте параметр по константной ссылке для копирования
  void SetExpectedData(const std::vector<uint8_t>& data) { expectedData = data; }

  void SetUpWH(int width_, int height_) {
    width = width_;
    height = height_;
  }

  void CreateGaussFilterHorizontal() { gaussFilterHorizontal = std::make_shared<GaussFilterHorizontal>(taskData); }

  void CommonSetUp() {
    inputData.resize(width * height * 3, 0);
    outputData.resize(width * height * 3, 0);
    expectedData.resize(width * height * 3, 0);
    taskData = std::make_shared<ppc::core::TaskData>();
  }

  void InitializeTaskData() {
    taskData->inputs.push_back(reinterpret_cast<uint8_t*>(inputData.data()));
    taskData->inputs_count.push_back(width);
    taskData->inputs_count.push_back(height);
    taskData->outputs.push_back(reinterpret_cast<uint8_t*>(outputData.data()));
    taskData->outputs_count.push_back(width);
    taskData->outputs_count.push_back(height);
  }

  void FillChannel(size_t channel, uint8_t value) {
    for (size_t i = channel; i < inputData.size(); i += 3) {
      inputData[i] = value;
    }
  }

  void ValidateOutputData() { ASSERT_EQ(expectedData, outputData); }

  static std::shared_ptr<ppc::core::PerfAttr> SetUpPerfAttributes() {
    auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
    perfAttr->num_running = 10;
    return perfAttr;
  }

  void static RunPerfAnalysis(const std::shared_ptr<GaussFilterHorizontal>& filter) {
    auto perfAttr = SetUpPerfAttributes();
    const auto t0 = std::chrono::high_resolution_clock::now();
    perfAttr->current_timer = [&] {
      auto current_time_point = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
      return static_cast<double>(duration) * 1e-9;
    };
    auto perfResults = std::make_shared<ppc::core::PerfResults>();
    auto perfAnalyzer = std::make_shared<ppc::core::Perf>(filter);
    perfAnalyzer->pipeline_run(perfAttr, perfResults);
    ppc::core::Perf::print_perf_statistic(perfResults);
  }
};

TEST_F(filatov_m_linear_image_filtering_perf_tbb, test_pipeline_run_with_red_image) {
  SetUpWH(512, 512);
  CommonSetUp();
  FillChannel(0, 111);
  CreateGaussFilterHorizontal();
  SetExpectedData(inputData);
  InitializeTaskData();
  RunPerfAnalysis(gaussFilterHorizontal);
  ValidateOutputData();
}

TEST_F(filatov_m_linear_image_filtering_perf_tbb, test_task_run_with_blue_image) {
  SetUpWH(512, 512);
  CommonSetUp();
  FillChannel(1, 255);
  CreateGaussFilterHorizontal();
  SetExpectedData(inputData);
  InitializeTaskData();
  RunPerfAnalysis(gaussFilterHorizontal);
  ValidateOutputData();
}
