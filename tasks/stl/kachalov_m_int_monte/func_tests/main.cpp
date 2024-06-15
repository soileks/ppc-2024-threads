// Copyright 2024 Kachalov Mikhail
#include <gtest/gtest.h>

#include <cmath>
#include <memory>
#include <thread>
#include <vector>

#include "stl/kachalov_m_int_monte/include/ops_stl.hpp"
#define ESTIMATE 0.2

using integ_func = double (*)(double, double);
double mult_xy(double x, double y) { return x * y; }
double Integral_exp(double x, double y) { return exp(x) + exp(y); }
double Integral_log(double x, double y) { return log(x + 1) + log(y + 1); }
double Integral_sinh(double x, double y) { return sinh(x) + sinh(y); }
double Integral_cosh(double x, double y) { return cosh(x) + cosh(y); }

TEST(kachalov_m_int_monte_stl, Test_mult) {
  double res = 2.25;
  integ_func f = mult_xy;

  // Create data
  std::vector<double> in1 = {0, 1};
  std::vector<double> in2 = {0, 3};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_stl(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(in1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(in2.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(in2.size());

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task
  KachalovIntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(in1.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(in2.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataStl->inputs_count.emplace_back(in2.size());

  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_stl.data()));
  taskDataStl->outputs_count.emplace_back(out_stl.size());

  KachalovIntegralSTLMonteCarlo testTaskStl(taskDataStl);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();

  ASSERT_NEAR(out_seq[0], out_stl[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_stl, Test_exp) {
  double res = 36.9453;
  integ_func f = Integral_exp;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_stl(1, res);

  // Create TaskData for sequential task
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(in1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(in2.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(in2.size());

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task for sequential task
  KachalovIntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData for STL task
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(in1.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(in2.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataStl->inputs_count.emplace_back(in2.size());

  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_stl.data()));
  taskDataStl->outputs_count.emplace_back(out_stl.size());

  KachalovIntegralSTLMonteCarlo testTaskStl(taskDataStl);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();

  ASSERT_NEAR(out_seq[0], out_stl[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_stl, Test_log) {
  double res = 2.88539;
  integ_func f = Integral_log;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_stl(1, res);

  // Create TaskData for sequential task
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(in1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(in2.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(in2.size());

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task for sequential task
  KachalovIntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData for STL task
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(in1.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(in2.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataStl->inputs_count.emplace_back(in2.size());

  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_stl.data()));
  taskDataStl->outputs_count.emplace_back(out_stl.size());

  KachalovIntegralSTLMonteCarlo testTaskStl(taskDataStl);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();

  ASSERT_NEAR(out_seq[0], out_stl[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_stl, Test_sinh) {
  double res = 10.0179;
  integ_func f = Integral_sinh;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_stl(1, res);

  // Create TaskData for sequential task
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(in1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(in2.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(in2.size());

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task for sequential task
  KachalovIntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData for STL task
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(in1.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(in2.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataStl->inputs_count.emplace_back(in2.size());

  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_stl.data()));
  taskDataStl->outputs_count.emplace_back(out_stl.size());

  KachalovIntegralSTLMonteCarlo testTaskStl(taskDataStl);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();

  ASSERT_NEAR(out_seq[0], out_stl[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_stl, Test_cosh) {
  double res = 27.3082;
  integ_func f = Integral_cosh;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_stl(1, res);

  // Create TaskData for sequential task
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(in1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(in2.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(in2.size());

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task for sequential task
  KachalovIntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData for STL task
  std::shared_ptr<ppc::core::TaskData> taskDataStl = std::make_shared<ppc::core::TaskData>();
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataStl->inputs_count.emplace_back(in1.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataStl->inputs_count.emplace_back(in2.size());
  taskDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataStl->inputs_count.emplace_back(in2.size());

  taskDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_stl.data()));
  taskDataStl->outputs_count.emplace_back(out_stl.size());

  KachalovIntegralSTLMonteCarlo testTaskStl(taskDataStl);
  ASSERT_EQ(testTaskStl.validation(), true);
  testTaskStl.pre_processing();
  testTaskStl.run();
  testTaskStl.post_processing();

  ASSERT_NEAR(out_seq[0], out_stl[0], ESTIMATE);
}