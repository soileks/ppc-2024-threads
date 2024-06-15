// Copyright 2024 Fedotov Kirill
#include <gtest/gtest.h>

#include <vector>

#include "stl/fedotov_k_monte_carlo_method/include/ops_stl.hpp"

using namespace fedotov_stl;

TEST(fedotov_k_test, Function_x_plus_y_test1) {
  double result_of_integration = 16;
  double (*f)(double, double) = func_x_plus_y;

  // Create data
  std::vector<double> first_integral_limit = {1, 3};
  std::vector<double> second_integral_limit = {1, 3};
  std::vector<double> output(1, result_of_integration);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataForSeqTask = std::make_shared<ppc::core::TaskData>();
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(first_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(second_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  dataForSeqTask->inputs_count.emplace_back(sizeof(f));

  dataForSeqTask->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  dataForSeqTask->outputs_count.emplace_back(output.size());

  // Create Task
  FedotovTaskSeq taskSeq(dataForSeqTask);
  ASSERT_EQ(taskSeq.validation(), true);
  taskSeq.pre_processing();
  taskSeq.run();
  taskSeq.post_processing();
  ASSERT_NEAR(result_of_integration, output[0], 0.5);
}

TEST(fedotov_k_test, Function_x_plus_y_test2) {
  double result_of_integration = 0;
  double (*f)(double, double) = func_x_plus_y;

  // Create data
  std::vector<double> first_integral_limit = {0, 0};
  std::vector<double> second_integral_limit = {0, 0};
  std::vector<double> output(1, result_of_integration);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataForSeqTask = std::make_shared<ppc::core::TaskData>();
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(first_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(second_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  dataForSeqTask->inputs_count.emplace_back(sizeof(f));

  dataForSeqTask->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  dataForSeqTask->outputs_count.emplace_back(output.size());

  // Create Task
  FedotovTaskSeq taskSeq(dataForSeqTask);
  ASSERT_EQ(taskSeq.validation(), true);
  taskSeq.pre_processing();
  taskSeq.run();
  taskSeq.post_processing();
  ASSERT_NEAR(result_of_integration, output[0], 0.5);
}

TEST(fedotov_k_test, Function_x_plus_y_test3) {
  double result_of_integration = 125;
  double (*f)(double, double) = func_x_plus_y;

  // Create data
  std::vector<double> first_integral_limit = {0, 5};
  std::vector<double> second_integral_limit = {0, 5};
  std::vector<double> output(1, result_of_integration);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataForSeqTask = std::make_shared<ppc::core::TaskData>();
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(first_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(second_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  dataForSeqTask->inputs_count.emplace_back(sizeof(f));

  dataForSeqTask->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  dataForSeqTask->outputs_count.emplace_back(output.size());

  // Create Task
  FedotovTaskSeq taskSeq(dataForSeqTask);
  ASSERT_EQ(taskSeq.validation(), true);
  taskSeq.pre_processing();
  taskSeq.run();
  taskSeq.post_processing();
  ASSERT_NEAR(result_of_integration, output[0], 0.5);
}

TEST(fedotov_k_test, Function_x_plus_y_test4) {
  double result_of_integration = 45;
  double (*f)(double, double) = func_x_plus_y;

  // Create data
  std::vector<double> first_integral_limit = {2, 5};
  std::vector<double> second_integral_limit = {3, 5};
  std::vector<double> output(1, result_of_integration);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataForSeqTask = std::make_shared<ppc::core::TaskData>();
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(first_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(second_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  dataForSeqTask->inputs_count.emplace_back(sizeof(f));

  dataForSeqTask->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  dataForSeqTask->outputs_count.emplace_back(output.size());

  // Create Task
  FedotovTaskSeq taskSeq(dataForSeqTask);
  ASSERT_EQ(taskSeq.validation(), true);
  taskSeq.pre_processing();
  taskSeq.run();
  taskSeq.post_processing();
  ASSERT_NEAR(result_of_integration, output[0], 0.5);
}

TEST(fedotov_k_test, Function_x_plus_y_test5) {
  double result_of_integration = 15;
  double (*f)(double, double) = func_x_plus_y;

  // Create data
  std::vector<double> first_integral_limit = {2, 3};
  std::vector<double> second_integral_limit = {1, 4};
  std::vector<double> output(1, result_of_integration);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> dataForSeqTask = std::make_shared<ppc::core::TaskData>();
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(first_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_integral_limit.data()));
  dataForSeqTask->inputs_count.emplace_back(second_integral_limit.size());
  dataForSeqTask->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  dataForSeqTask->inputs_count.emplace_back(sizeof(f));

  dataForSeqTask->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  dataForSeqTask->outputs_count.emplace_back(output.size());

  // Create Task
  FedotovTaskSeq taskSeq(dataForSeqTask);
  ASSERT_EQ(taskSeq.validation(), true);
  taskSeq.pre_processing();
  taskSeq.run();
  taskSeq.post_processing();
  ASSERT_NEAR(result_of_integration, output[0], 0.5);
}