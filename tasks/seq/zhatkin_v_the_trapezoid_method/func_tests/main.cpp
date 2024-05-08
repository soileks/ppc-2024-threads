// Copyright 2024 Zhatkin Vyacheslav

#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include "seq/zhatkin_v_the_trapezoid_method/include/trapezoid_method.hpp"

TEST(zhatkin_v_trapezoid_seq, Test1) {
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
  ZhatkinTaskSequential testTaskSequential(taskDataSeq, sqr_x_y);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_NEAR(res, out[0], 0.02);
}

TEST(zhatkin_v_trapezoid_seq, Test2) {
  double lowerx = 0.0;
  double upperx = 1.0;
  double lowery = 0.0;
  double uppery = 1.0;
  int nx = 10;
  int ny = 10;
  double res = 0.20;

  auto mul_x_y = [](double x, double y) { return x * y; };

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
  ZhatkinTaskSequential testTaskSequential(taskDataSeq, mul_x_y);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_NEAR(res, out[0], 0.02);
}

TEST(zhatkin_v_trapezoid_seq, Test3) {
  double lowerx = 0.0;
  double upperx = 3.14159265358979323846;
  double lowery = 0.0;
  double uppery = 3.14159265358979323846;
  int nx = 100;
  int ny = 100;
  double res = 2 * 3.14159265358979323846;

  auto trigm_x_y = [](double x, double y) { return cos(x) + sin(y); };

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
  ZhatkinTaskSequential testTaskSequential(taskDataSeq, trigm_x_y);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_NEAR(res, out[0], 0.1);
}

TEST(zhatkin_v_trapezoid_seq, Test4) {
  const double lowerx = 0.0;
  double upperx = 3.14159265358979323846;
  double lowery = 0.0;
  double uppery = 1.0;
  int nx = 50;
  int ny = 50;
  double res = 2.18;

  auto trigmul_x_y = [](double x, double y) { return x * sin(y); };

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
  ZhatkinTaskSequential testTaskSequential(taskDataSeq, trigmul_x_y);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_NEAR(res, out[0], 0.1);
}

TEST(zhatkin_v_trapezoid_seq, Test5) {
  double lowerx = 1.0;
  double upperx = 2.0;
  double lowery = 1.0;
  double uppery = 2.0;
  int nx = 300;
  int ny = 300;
  double res = 1.039;

  auto divide_x_y = [](double x, double y) { return x / y; };

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
  ZhatkinTaskSequential testTaskSequential(taskDataSeq, divide_x_y);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_NEAR(res, out[0], 0.02);
}