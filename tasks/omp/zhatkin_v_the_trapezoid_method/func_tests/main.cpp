// Copyright 2024 Zhatkin Vyacheslav

#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include "omp/zhatkin_v_the_trapezoid_method/include/trapezoid_method_omp.hpp"

TEST(zhatkin_v_trapezoid_omp, Test1) {
  double lowerx = 0.0;
  double upperx = 0.0;
  double lowery = 0.0;
  double uppery = 0.0;
  int nx = 20;
  int ny = 20;
  double res = 0.0;

  auto sqr_x_y = [](double x, double y) { return sqrt(x * x + y * y); };

  // Create data
  std::vector<double> out(1, res);
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataSeq->inputs_count.emplace_back(n.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  ZhatkinTaskOMP testTaskSequential(taskDataSeq, sqr_x_y);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_NEAR(res, out[0], 0.02);

  // Create data
  std::vector<double> par_res(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataPar->inputs_count.emplace_back(n.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(par_res.data()));
  taskDataPar->outputs_count.emplace_back(par_res.size());

  // Create Task
  ZhatkinTaskOMP testOmpTaskParallel(taskDataPar, sqr_x_y);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  ASSERT_EQ(par_res[0], out[0]);
}

TEST(zhatkin_v_trapezoid_omp, Test2) {
  double lowerx = 0.0;
  double upperx = 1.0;
  double lowery = 0.0;
  double uppery = 1.0;
  int nx = 10;
  int ny = 10;
  double res = 0.20;

  auto productXY = [](double x, double y) { return x * y; };

  // Create data
  std::vector<double> out(1, res);
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataSeq->inputs_count.emplace_back(n.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  ZhatkinTaskOMP testTaskSequential(taskDataSeq, productXY);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create data
  std::vector<double> par_res(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataPar->inputs_count.emplace_back(n.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(par_res.data()));
  taskDataPar->outputs_count.emplace_back(par_res.size());

  // Create Task
  ZhatkinTaskOMP testOmpTaskParallel(taskDataPar, productXY);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  ASSERT_EQ(par_res[0], out[0]);
}

TEST(zhatkin_v_trapezoid_omp, Test3) {
  double lowerx = 0.0;
  double upperx = 3.14159265358979323846;
  double lowery = 0.0;
  double uppery = 3.14159265358979323846;
  int nx = 100;
  int ny = 100;
  double res = 2 * 3.14159265358979323846;

  auto sumCosSin = [](double x, double y) { return cos(x) + sin(y); };

  // Create data
  std::vector<double> out(1, res);
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataSeq->inputs_count.emplace_back(n.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  ZhatkinTaskOMP testTaskSequential(taskDataSeq, sumCosSin);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create data
  std::vector<double> par_res(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataPar->inputs_count.emplace_back(n.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(par_res.data()));
  taskDataPar->outputs_count.emplace_back(par_res.size());

  // Create Task
  ZhatkinTaskOMP testOmpTaskParallel(taskDataPar, sumCosSin);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  ASSERT_EQ(par_res[0], out[0]);
}

TEST(zhatkin_v_trapezoid_omp, Test4) {
  const double lowerx = 0.0;
  double upperx = 3.14159265358979323846;
  double lowery = 0.0;
  double uppery = 1.0;
  int nx = 50;
  int ny = 50;
  double res = 2.18;

  auto productXsinY = [](double x, double y) { return x * sin(y); };

  // Create data
  std::vector<double> out(1, res);
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataSeq->inputs_count.emplace_back(n.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  ZhatkinTaskOMP testTaskSequential(taskDataSeq, productXsinY);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  std::vector<double> par_res(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataPar->inputs_count.emplace_back(n.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(par_res.data()));
  taskDataPar->outputs_count.emplace_back(par_res.size());

  // Create Task
  ZhatkinTaskOMP testOmpTaskParallel(taskDataPar, productXsinY);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  ASSERT_EQ(par_res[0], out[0]);
}

TEST(zhatkin_v_trapezoid_seq, Test5) {
  double lowerx = 1.0;
  double upperx = 2.0;
  double lowery = 1.0;
  double uppery = 2.0;
  int nx = 300;
  int ny = 300;
  double res = 1.039;

  auto quotientXY = [](double x, double y) { return x / y; };

  // Create data
  std::vector<double> out(1, res);
  std::vector<double> in = {lowerx, upperx, lowery, uppery};
  std::vector<int> n = {nx, ny};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataSeq->inputs_count.emplace_back(n.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  ZhatkinTaskOMP testTaskSequential(taskDataSeq, quotientXY);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create data
  std::vector<double> par_res(1, res);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(n.data()));
  taskDataPar->inputs_count.emplace_back(n.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(par_res.data()));
  taskDataPar->outputs_count.emplace_back(par_res.size());

  // Create Task
  ZhatkinTaskOMP testOmpTaskParallel(taskDataPar, quotientXY);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  ASSERT_EQ(par_res[0], out[0]);
}
