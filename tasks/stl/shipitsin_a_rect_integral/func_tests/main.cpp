// Copyright 2024 Shipitsin Alex
#include <gtest/gtest.h>

#include <vector>

#include "stl/shipitsin_a_rect_integral/include/ops_stl.hpp"

using namespace shipitsin_stl;

TEST(shipitsin_a_rect_integral_stl, Test_x_sub_y_1) {
  int n = 100;
  Func f = shipitsin_flin;

  std::vector<double> in = {0, 2, 0, 2};
  std::vector<double> out_seq(1, 0);
  std::vector<double> out_par(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  ShipitsinTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  ShipitsinTaskSTL testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  ASSERT_NEAR(out_par[0], out_seq[0], 1e-3);
}

TEST(shipitsin_a_rect_integral_stl, Test_x_sub_y_2) {
  Func f = shipitsin_flin;
  int n = 100;

  std::vector<double> in = {-4, 2, 4, 8};
  std::vector<double> out_seq(1, 0);
  std::vector<double> out_par(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  ShipitsinTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  ShipitsinTaskSTL testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  ASSERT_NEAR(out_par[0], out_seq[0], 1e-3);
}

TEST(shipitsin_a_rect_integral_stl, Test_x_mul_y) {
  Func f = shipitsin_fxy;
  int n = 100;

  std::vector<double> in = {0, 2, 1, 2};
  std::vector<double> out_seq(1, 0);
  std::vector<double> out_par(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  ShipitsinTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  ShipitsinTaskSTL testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  ASSERT_NEAR(out_par[0], out_seq[0], 1e-3);
}

TEST(shipitsin_a_rect_integral_stl, Test_y_sin_x) {
  Func f = shipitsin_fysinx;
  int n = 100;

  std::vector<double> in = {-2, 2, 1, 2};
  std::vector<double> out_seq(1, 0);
  std::vector<double> out_par(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  ShipitsinTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  ShipitsinTaskSTL testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  ASSERT_NEAR(out_par[0], out_seq[0], 1e-3);
}

TEST(shipitsin_a_rect_integral_stl, Test_x_exp_y) {
  Func f = shipitsin_fxexpy;
  int n = 1000;

  std::vector<double> in = {0, 4, 1, 2};
  std::vector<double> out_seq(1, 0);
  std::vector<double> out_par(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs_count.emplace_back(1);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  ShipitsinTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(f));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs_count.emplace_back(1);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  ShipitsinTaskSTL testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  ASSERT_NEAR(out_par[0], out_seq[0], 1e-3);
}