// Copyright 2024 Kachalov Mikhail
#include <gtest/gtest.h>
#include <tbb/tbb.h>

#include <cmath>
#include <vector>

#include "tbb/kachalov_m_int_monte/include/ops_tbb.hpp"
#define ESTIMATE 0.2

using integ_func = double (*)(double, double);
double Integral_sum(double x, double y) { return x + y; }
double Integral_sin(double x, double y) { return sin(x) + sin(y); }
double Integral_cos(double x, double y) { return cos(x) + cos(y); }
double mult_xy(double x, double y) { return x * y; }
double Integral_mult_sqry_fxyy(double x, double y) { return x * y * y; }
double Integral_exp(double x, double y) { return exp(x) + exp(y); }
double Integral_log(double x, double y) { return log(x + 1) + log(y + 1); }
double Integral_sinh(double x, double y) { return sinh(x) + sinh(y); }
double Integral_cosh(double x, double y) { return cosh(x) + cosh(y); }

TEST(kachalov_m_int_monte_tbb, Test_mult) {
  double res = 2.25;
  integ_func f = mult_xy;

  // Create data
  std::vector<double> in1 = {0, 1};
  std::vector<double> in2 = {0, 3};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_tbb(1, res);

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
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataTbb->inputs_count.emplace_back(in2.size());

  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_tbb.data()));
  taskDataTbb->outputs_count.emplace_back(out_tbb.size());

  KachalovIntegralTBBMonteCarlo testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();

  ASSERT_NEAR(out_seq[0], out_tbb[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_tbb, Test_exp) {
  double res = 36.9453;
  integ_func f = Integral_exp;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_tbb(1, res);

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

  // Create TaskData for TBB task
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataTbb->inputs_count.emplace_back(in2.size());

  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_tbb.data()));
  taskDataTbb->outputs_count.emplace_back(out_tbb.size());

  KachalovIntegralTBBMonteCarlo testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();

  ASSERT_NEAR(out_seq[0], out_tbb[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_tbb, Test_log) {
  double res = 2.88539;
  integ_func f = Integral_log;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_tbb(1, res);

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

  // Create TaskData for TBB task
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataTbb->inputs_count.emplace_back(in2.size());

  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_tbb.data()));
  taskDataTbb->outputs_count.emplace_back(out_tbb.size());

  KachalovIntegralTBBMonteCarlo testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();

  ASSERT_NEAR(out_seq[0], out_tbb[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_tbb, Test_sinh) {
  double res = 10.0179;
  integ_func f = Integral_sinh;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_tbb(1, res);

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

  // Create TaskData for TBB task
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataTbb->inputs_count.emplace_back(in2.size());

  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_tbb.data()));
  taskDataTbb->outputs_count.emplace_back(out_tbb.size());

  KachalovIntegralTBBMonteCarlo testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();

  ASSERT_NEAR(out_seq[0], out_tbb[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_tbb, Test_cosh) {
  double res = 27.3082;
  integ_func f = Integral_cosh;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_tbb(1, res);

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

  // Create TaskData for TBB task
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataTbb->inputs_count.emplace_back(in1.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataTbb->inputs_count.emplace_back(in2.size());
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataTbb->inputs_count.emplace_back(in2.size());

  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_tbb.data()));
  taskDataTbb->outputs_count.emplace_back(out_tbb.size());

  KachalovIntegralTBBMonteCarlo testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();

  ASSERT_NEAR(out_seq[0], out_tbb[0], ESTIMATE);
}