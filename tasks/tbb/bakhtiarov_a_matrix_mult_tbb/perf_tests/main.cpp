// Copyright 2024 Bakhtiarov Alexander
#include <gtest/gtest.h>
#include <oneapi/tbb.h>
#include <chrono>
#include <vector>
#include "core/perf/include/perf.hpp"
#include "tbb/bakhtiarov_a_matrix_mult_css_tbb/include/ccs_matrix_mult.hpp"

using namespace std;

std::vector<double> create_matrix(size_t rows, size_t cols, std::function<bool(size_t, size_t)> fill_condition) {
  std::vector<double> matrix(rows * cols, 0.0);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (fill_condition(i, j)) {
        matrix[i * cols + j] = 1.0;
      }
    }
  }
  return matrix;
}

void test_matrix_multiplication(size_t p, size_t q, size_t r, const std::shared_ptr<SparseTBBMatrixMulti>& task) {
  auto lhs_in = create_matrix(p, q, [](size_t i, size_t) { return i % 4 == 0; });
  auto rhs_in = create_matrix(q, r, [](size_t, size_t j) { return j % 5 == 0; });
  std::vector<double> out(p * r, 0.0);

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(lhs_in.data()));
  taskData->inputs_count = {p, q};
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(rhs_in.data()));
  taskData->inputs_count.insert(taskData->inputs_count.end(), {q, r});
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
  taskData->outputs_count = {p, r};

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(task);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < p; ++i) {
    for (size_t j = 0; j < r; ++j) {
      if (i % 4 == 0 && j % 5 == 0)
        EXPECT_DOUBLE_EQ(out[i * r + j], q);
      else
        EXPECT_DOUBLE_EQ(out[i * r + j], 0.0);
    }
  }
}

TEST(bakhtiarov_a_matrix_mult_css_tbb, test_pipeline_run) {
  auto task = std::make_shared<SparseTBBMatrixMultiParallel>(std::make_shared<ppc::core::TaskData>());
  test_matrix_multiplication(500, 500, 500, task);
}

TEST(bakhtiarov_a_matrix_mult_css_tbb, test_task_run) {
  auto task = std::make_shared<SparseTBBMatrixMultiParallel>(std::make_shared<ppc::core::TaskData>());
  test_matrix_multiplication(700, 700, 700, task);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
