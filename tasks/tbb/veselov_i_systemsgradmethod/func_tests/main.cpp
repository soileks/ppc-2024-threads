// Copyright 2024 Veselov Ilya

#include <gtest/gtest.h>

#include <iostream>
#include <random>
#include <vector>

#include "tbb/veselov_i_systemsgradmethod/include/systemsgradmethod_tbb.hpp"

using namespace veselov_i_tbb;

TEST(veselov_i_systems_grad_method_tbb, Test_some_matrix) {
  int rows = 2;
  std::vector<double> matrix = {2.0, 1.0, 1.0, 2.0};
  std::vector<double> vec = {1.0, 3.0};
  std::vector<double> res(rows, 0.0);
  std::vector<double> excepted_res = {-0.3333333, 1.6666666};

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix.data()));
  taskDataTbb->inputs_count.emplace_back(rows * rows);
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataTbb->inputs_count.emplace_back(vec.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&rows));
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataTbb->outputs_count.emplace_back(vec.size());

  SystemsGradMethodTbb systemsGradMethodTbb(taskDataTbb);
  ASSERT_EQ(systemsGradMethodTbb.validation(), true);
  ASSERT_TRUE(systemsGradMethodTbb.pre_processing());
  ASSERT_TRUE(systemsGradMethodTbb.run());
  ASSERT_TRUE(systemsGradMethodTbb.post_processing());
  for (size_t i = 0; i < res.size(); i++) {
    ASSERT_LE(abs(excepted_res[i] - res[i]), 1e-6);
  }
}

TEST(veselov_i_systems_grad_method_tbb, Test_unit_matrix) {
  int rows = 4;
  std::vector<double> matrix = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  std::vector<double> vec = {1, 2, 3, 4};
  std::vector<double> res(rows, 0.0);
  std::vector<double> excepted_res = {1, 2, 3, 4};

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix.data()));
  taskDataTbb->inputs_count.emplace_back(rows * rows);
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataTbb->inputs_count.emplace_back(vec.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&rows));
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataTbb->outputs_count.emplace_back(vec.size());

  SystemsGradMethodTbb systemsGradMethodTbb(taskDataTbb);
  ASSERT_EQ(systemsGradMethodTbb.validation(), true);
  systemsGradMethodTbb.pre_processing();
  systemsGradMethodTbb.run();
  systemsGradMethodTbb.post_processing();
  for (size_t i = 0; i < res.size(); i++) {
    ASSERT_LE(abs(excepted_res[i] - res[i]), 1e-6);
  }
}

TEST(veselov_i_systems_grad_method_tbb, Test_random_10) {
  int rows = 10;
  std::vector<double> matrix = genRandomMatrix(rows, 10);
  std::vector<double> vec = genRandomVector(rows, 10);
  std::vector<double> res(rows, 0.0);

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix.data()));
  taskDataTbb->inputs_count.emplace_back(matrix.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataTbb->inputs_count.emplace_back(vec.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&rows));
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataTbb->outputs_count.emplace_back(res.size());

  SystemsGradMethodTbb systemsGradMethodTbb(taskDataTbb);
  ASSERT_EQ(systemsGradMethodTbb.validation(), true);
  systemsGradMethodTbb.pre_processing();
  systemsGradMethodTbb.run();
  systemsGradMethodTbb.post_processing();
  ASSERT_TRUE(checkSolution(matrix, vec, res));
}

TEST(veselov_i_systems_grad_method_tbb, Test_random_5) {
  int rows = 5;
  std::vector<double> matrix = genRandomMatrix(rows, 10);
  std::vector<double> vec = genRandomVector(rows, 10);
  std::vector<double> res(rows);

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix.data()));
  taskDataTbb->inputs_count.emplace_back(matrix.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataTbb->inputs_count.emplace_back(vec.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&rows));
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataTbb->outputs_count.emplace_back(res.size());

  SystemsGradMethodTbb systemsGradMethodTbb(taskDataTbb);
  ASSERT_EQ(systemsGradMethodTbb.validation(), true);
  systemsGradMethodTbb.pre_processing();
  systemsGradMethodTbb.run();
  systemsGradMethodTbb.post_processing();
  ASSERT_TRUE(checkSolution(matrix, vec, res));
}

TEST(veselov_i_systems_grad_method_tbb, Test_triple_diag_matrix) {
  int rows = 4;
  std::vector<double> matrix = {2, -1, 0, 0, -1, 2, -1, 0, 0, -1, 2, -1, 0, 0, -1, 2};
  std::vector<double> vec = {1, 2, 3, 4};
  std::vector<double> res(rows);
  std::vector<double> excepted_res = {4, 7, 8, 6};

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(matrix.data()));
  taskDataTbb->inputs_count.emplace_back(matrix.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataTbb->inputs_count.emplace_back(vec.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(&rows));
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataTbb->outputs_count.emplace_back(vec.size());

  SystemsGradMethodTbb systemsGradMethodTbb(taskDataTbb);
  ASSERT_EQ(systemsGradMethodTbb.validation(), true);
  systemsGradMethodTbb.pre_processing();
  systemsGradMethodTbb.run();
  systemsGradMethodTbb.post_processing();
  for (size_t i = 0; i < res.size(); i++) {
    ASSERT_LE(abs(excepted_res[i] - res[i]), 1e-6);
  }
}
