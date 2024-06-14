// Copyright 2024 Kachalov Mikhail
#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include "omp/kachalov_m_int_monte/include/ops_omp.hpp"
#define ESTIMATE 0.2

using integ_func = double (*)(double, double);
double Integral_sum(double x, double y) { return x + y; }
double Integral_sin(double x, double y) { return sin(x) + sin(y); }
double Integral_cos(double x, double y) { return cos(x) + cos(y); }
double mult_xy(double x, double y) { return x * y; }
double Integral_mult_sqry_fxyy(double x, double y) { return x * y * y; }

TEST(kachalov_m_int_monte_omp, Test_mult) {
  double res = 2.25;
  integ_func f = mult_xy;

  // Create data
  std::vector<double> in1 = {0, 1};
  std::vector<double> in2 = {0, 3};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_omp(1, res);

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
  IntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataOmp->inputs_count.emplace_back(in1.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataOmp->inputs_count.emplace_back(in2.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataOmp->inputs_count.emplace_back(in2.size());

  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_omp.data()));
  taskDataOmp->outputs_count.emplace_back(out_omp.size());

  IntegralOMPMonteCarlo testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  ASSERT_NEAR(out_seq[0], out_omp[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_omp, Test_sin) {
  double res = 5.67369;
  integ_func f = Integral_sin;

  // Create data
  std::vector<double> in1 = {-1, 2};
  std::vector<double> in2 = {-1, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_omp(1, res);

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
  IntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataOmp->inputs_count.emplace_back(in1.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataOmp->inputs_count.emplace_back(in2.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataOmp->inputs_count.emplace_back(in2.size());

  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_omp.data()));
  taskDataOmp->outputs_count.emplace_back(out_omp.size());

  IntegralOMPMonteCarlo testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  ASSERT_NEAR(out_seq[0], out_omp[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_omp, Test_cos) {
  double res = 6.22943;
  integ_func f = Integral_cos;

  // Create data
  std::vector<double> in1 = {-1, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_omp(1, res);

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
  IntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataOmp->inputs_count.emplace_back(in1.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataOmp->inputs_count.emplace_back(in2.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataOmp->inputs_count.emplace_back(in2.size());

  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_omp.data()));
  taskDataOmp->outputs_count.emplace_back(out_omp.size());

  IntegralOMPMonteCarlo testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  ASSERT_NEAR(out_seq[0], out_omp[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_omp, Test_int_sum) {
  double res = 8;
  integ_func f = Integral_sum;

  // Create data
  std::vector<double> in1 = {0, 2};
  std::vector<double> in2 = {0, 2};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_omp(1, res);

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
  IntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataOmp->inputs_count.emplace_back(in1.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataOmp->inputs_count.emplace_back(in2.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataOmp->inputs_count.emplace_back(in2.size());

  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_omp.data()));
  taskDataOmp->outputs_count.emplace_back(out_omp.size());

  IntegralOMPMonteCarlo testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  ASSERT_NEAR(out_seq[0], out_omp[0], ESTIMATE);
}

TEST(kachalov_m_int_monte_omp, Test_mult_sqry) {
  double res = 1.5;
  integ_func f = Integral_mult_sqry_fxyy;

  // Create data
  std::vector<double> in1 = {0, 3};
  std::vector<double> in2 = {0, 1};
  std::vector<double> out_seq(1, res);
  std::vector<double> out_omp(1, res);

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
  IntegralSequentialMonteCarlo testTaskSeq(taskDataSeq);
  ASSERT_EQ(testTaskSeq.validation(), true);
  testTaskSeq.pre_processing();
  testTaskSeq.run();
  testTaskSeq.post_processing();

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataOmp->inputs_count.emplace_back(in1.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataOmp->inputs_count.emplace_back(in2.size());
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataOmp->inputs_count.emplace_back(in2.size());

  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_omp.data()));
  taskDataOmp->outputs_count.emplace_back(out_omp.size());

  IntegralOMPMonteCarlo testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  ASSERT_NEAR(out_seq[0], out_omp[0], ESTIMATE);
}