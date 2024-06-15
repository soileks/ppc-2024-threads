// Copyright 2024 Bakhtiarov Alexander
#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <future>
#include <numeric>
#include <thread>
#include <vector>

#include "stl/bakhtiarov_a_matrix_mult_stl/include/ccs_matrix_mult.hpp"

TEST(bakhtiarov_a_matrix_mult_stl, test_size) {
  size_t n1 = 4;
  size_t m1 = 5;
  size_t n2 = 3;
  size_t m2 = 4;

  // Create data
  std::vector<double> in1(n1 * m1);
  std::vector<double> in2(n2 * m2);
  std::vector<double> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(n1);
  taskDataStl->inputs_count.emplace_back(m1);
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(n2);
  taskDataStl->inputs_count.emplace_back(m2);
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataStl->outputs_count.emplace_back(n1);
  taskDataStl->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixMultiSTL sparseMatrixMultiSTL(taskDataStl);
  ASSERT_FALSE(sparseMatrixMultiSTL.validation());
}

TEST(bakhtiarov_a_matrix_mult_stl, test_multy_correct) {
  size_t n1 = 4;
  size_t m1 = 4;
  size_t n2 = 4;
  size_t m2 = 4;

  // Create data
  std::vector<double> in1{5, 0, 0, 0, 0, 0, 5, 0, 0, 1, 0, 0, 8, 0, 6, 0};
  std::vector<double> in2{5, 0, 0, 8, 0, 0, 1, 0, 0, 5, 0, 6, 0, 0, 0, 0};
  std::vector<double> out(n1 * m2);
  std::vector<double> test{25, 0, 0, 40, 0, 25, 0, 30, 0, 0, 1, 0, 40, 30, 0, 100};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(n1);
  taskDataStl->inputs_count.emplace_back(m1);
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(n2);
  taskDataStl->inputs_count.emplace_back(m2);
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataStl->outputs_count.emplace_back(n1);
  taskDataStl->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixMultiSTL sparseMatrixMultiSTL(taskDataStl);
  sparseMatrixMultiSTL.validation();
  sparseMatrixMultiSTL.pre_processing();
  sparseMatrixMultiSTL.run();
  sparseMatrixMultiSTL.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i] == test[i]) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);
}

TEST(bakhtiarov_a_matrix_mult_stl, test_multy_correct2) {
  size_t n1 = 4;
  size_t m1 = 4;
  size_t n2 = 4;
  size_t m2 = 4;

  // Create data
  std::vector<double> in1{5, 0, 0, 0, 0, 0, 5, 0, 0, 1, 0, 0, 8, 0, 6, 0};
  std::vector<double> in2{5, 0, 0, 8, 0, 0, 1, 0, 0, 5, 0, 6, 0, 0, 0, 0};
  std::vector<double> out(n1 * m2);
  std::vector<double> test{25, 0, 0, 40, 0, 25, 0, 30, 0, 0, 1, 0, 40, 30, 0, 100};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(n1);
  taskDataStl->inputs_count.emplace_back(m1);
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(n2);
  taskDataStl->inputs_count.emplace_back(m2);
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataStl->outputs_count.emplace_back(n1);
  taskDataStl->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixMultiSTL sparseMatrixMultiSTL(taskDataStl);
  sparseMatrixMultiSTL.validation();
  sparseMatrixMultiSTL.pre_processing();
  sparseMatrixMultiSTL.run();
  sparseMatrixMultiSTL.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i] == test[i]) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);
}

TEST(bakhtiarov_a_matrix_mult_stl, inverse_matrix) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;

  // Create data
  std::vector<double> in1{1, -2, 1, 2, 1, -1, 3, 2, -2};
  std::vector<double> in2{0, 2, -1, -1, 5, -3, -1, 8, -5};
  std::vector<double> out(n1 * m2);
  std::vector<double> identity{1, 0, 0, 0, 1, 0, 0, 0, 1};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(n1);
  taskDataStl->inputs_count.emplace_back(m1);
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(n2);
  taskDataStl->inputs_count.emplace_back(m2);
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataStl->outputs_count.emplace_back(n1);
  taskDataStl->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixMultiSTL sparseMatrixMultiSTL(taskDataStl);
  sparseMatrixMultiSTL.validation();
  sparseMatrixMultiSTL.pre_processing();
  sparseMatrixMultiSTL.run();
  sparseMatrixMultiSTL.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i] == identity[i]) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);
}

TEST(bakhtiarov_a_matrix_mult_stl, zero_matrix) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;

  // Create data
  std::vector<double> in1{1, -2, 1, 2, 1, -1, 3, 2, -2};
  std::vector<double> in2(n2 * m2, 0);
  std::vector<double> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(n1);
  taskDataStl->inputs_count.emplace_back(m1);
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(n2);
  taskDataStl->inputs_count.emplace_back(m2);
  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataStl->outputs_count.emplace_back(n1);
  taskDataStl->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixMultiSTL sparseMatrixMultiSTL(taskDataStl);
  sparseMatrixMultiSTL.validation();
  sparseMatrixMultiSTL.pre_processing();
  sparseMatrixMultiSTL.run();
  sparseMatrixMultiSTL.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i] == 0.0) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);
}
