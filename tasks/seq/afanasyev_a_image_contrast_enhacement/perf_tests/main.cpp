// Copyright 2024 Afanasyev Aleksey
#include <gtest/gtest.h>

#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/afanasyev_a_image_contrast_enhacement/include/image_contrast_enhacement_task.hpp"

using namespace AfanasyevAlekseySeq;

TEST(afanasyev_a_image_contrast_enhacement_seq, test_pipeline_run) {
  std::vector<Pixel> input_pixels = generateRandomPixels(1'000'000);
  std::vector<Pixel> output_pixels(input_pixels.size());
  std::vector<Pixel> expected_output_pixels(input_pixels.size());
  bool ok = contrastEnhancementSeq(input_pixels, expected_output_pixels);
  ASSERT_TRUE(ok);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  auto task = std::make_shared<ImageContrastEnhacementTask>(taskData);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(task);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (std::size_t i = 0; i < output_pixels.size(); i++) {
    ASSERT_EQ(output_pixels[i], expected_output_pixels[i]);
  }
}

TEST(afanasyev_a_image_contrast_enhacement_seq, test_task_run) {
  std::vector<Pixel> input_pixels = generateRandomPixels(1'000'000);
  std::vector<Pixel> output_pixels(input_pixels.size());
  std::vector<Pixel> expected_output_pixels(input_pixels.size());
  bool ok = contrastEnhancementSeq(input_pixels, expected_output_pixels);
  ASSERT_TRUE(ok);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  auto task = std::make_shared<ImageContrastEnhacementTask>(taskData);

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
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(task);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (std::size_t i = 0; i < output_pixels.size(); i++) {
    ASSERT_EQ(output_pixels[i], expected_output_pixels[i]);
  }
}
