// Copyright 2024 Kochetov Nikolay
#include <gtest/gtest.h>

#include <random>
#include <vector>
#include <chrono>

#include "core/perf/include/perf.hpp"
#include "seq/kochetov_n_gauss_filter/include/gauss_filter.hpp"

TEST(kochetov_n_gauss_filter_seq_perf, test_pipeline_run) {
  int size = 1000;
  int* image = new int[size * size];
  for (int i = 0; i < size * size; i++) {
    image[i] = 255;
  }

  int* output = new int[size * size];

  int* result = new int[size * size];
  for (int i = 0; i < size * size; i++) {
    if (i < size || i % size == 0 || i > size * size - size || i % size == size - 1) {
      result[i] = 0;
    }
    else {
      result[i] = 255;
    }
  }

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(image));
  taskData->inputs_count.emplace_back(size);
  taskData->inputs_count.emplace_back(size);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output));
  taskData->outputs_count.emplace_back(size);
  taskData->outputs_count.emplace_back(size);

  auto gaussFilter = std::make_shared<GaussFilter>(taskData);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(gaussFilter);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (int i = 0; i < size * size; i++) {
    EXPECT_EQ(output[i], result[i]);
  }
}

TEST(kochetov_n_gauss_filter_seq_perf, test_task_run) {
  int size = 1000;
  int* image = new int[size * size];
  for (int i = 0; i < size * size; i++) {
    image[i] = 255;
  }

  int* output = new int[size * size];

  int* result = new int[size * size];
  for (int i = 0; i < size * size; i++) {
    if (i < size || i % size == 0 || i > size * size - size || i % size == size - 1) {
      result[i] = 0;
    }
    else {
      result[i] = 255;
    }
  }

  auto taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(image));
  taskData->inputs_count.emplace_back(size);
  taskData->inputs_count.emplace_back(size);
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output));
  taskData->outputs_count.emplace_back(size);
  taskData->outputs_count.emplace_back(size);

  auto gaussFilter = std::make_shared<GaussFilter>(taskData);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(gaussFilter);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (int i = 0; i < size * size; i++) {
    EXPECT_EQ(output[i], result[i]);
  }
}
