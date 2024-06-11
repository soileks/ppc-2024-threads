// Copyright 2024 Nedelin Dmitry
#include <gtest/gtest.h>

#include"stl/nedelin_d_stl_cannons_algorithm/include/ops_stl.hpp"

TEST(nedelin_d_block_cannons_stl, Multiply_3x3) {
  int n = 3;
  int m = 3;

  std::vector<double> in_mtrx_A = RndMatrix(n, m);
  std::vector<double> in_mtrx_B = RndMatrix(n, m);
  std::vector<double> seq_block_result(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_block_result.data()));
  taskDataSeq->outputs_count.emplace_back(seq_block_result.size());

  TestTBBSequentialNedelinCannon testTBBTaskSequential(taskDataSeq);
  ASSERT_EQ(testTBBTaskSequential.validation(), true);
  testTBBTaskSequential.pre_processing();
  testTBBTaskSequential.run();
  testTBBTaskSequential.post_processing();

  std::vector<double> parallel_res(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(parallel_res.data()));
  taskDataPar->outputs_count.emplace_back(parallel_res.size());

  TestTBBSequentialNedelinCannon testSTLTaskParallel(taskDataPar);
  ASSERT_EQ(testSTLTaskParallel.validation(), true);
  testSTLTaskParallel.pre_processing();
  testSTLTaskParallel.run();
  testSTLTaskParallel.post_processing();

  for (size_t i = 0; i < seq_block_result.size(); ++i) {
    ASSERT_EQ(parallel_res[i], seq_block_result[i]);
  }
}

TEST(nedelin_d_block_cannons_stl, Multiply_2x2) {
  int n = 2;
  int m = 2;

  std::vector<double> in_mtrx_A = RndMatrix(n, m);
  std::vector<double> in_mtrx_B = RndMatrix(n, m);
  std::vector<double> seq_block_result(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_block_result.data()));
  taskDataSeq->outputs_count.emplace_back(seq_block_result.size());

  TestTBBSequentialNedelinCannon testTBBTaskSequential(taskDataSeq);
  ASSERT_EQ(testTBBTaskSequential.validation(), true);
  testTBBTaskSequential.pre_processing();
  testTBBTaskSequential.run();
  testTBBTaskSequential.post_processing();

  std::vector<double> parallel_res(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(parallel_res.data()));
  taskDataPar->outputs_count.emplace_back(parallel_res.size());

  TestTBBSequentialNedelinCannon testSTLTaskParallel(taskDataPar);
  ASSERT_EQ(testSTLTaskParallel.validation(), true);
  testSTLTaskParallel.pre_processing();
  testSTLTaskParallel.run();
  testSTLTaskParallel.post_processing();

  for (size_t i = 0; i < seq_block_result.size(); ++i) {
    ASSERT_EQ(parallel_res[i], seq_block_result[i]);
  }
}

TEST(nedelin_d_block_cannons_stl, Multiply_5x5) {
  int n = 5;
  int m = 5;

  std::vector<double> in_mtrx_A = RndMatrix(n, m);
  std::vector<double> in_mtrx_B = RndMatrix(n, m);
  std::vector<double> seq_block_result(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_block_result.data()));
  taskDataSeq->outputs_count.emplace_back(seq_block_result.size());

  TestTBBSequentialNedelinCannon testOmpTaskSequential(taskDataSeq);
  ASSERT_EQ(testOmpTaskSequential.validation(), true);
  testOmpTaskSequential.pre_processing();
  testOmpTaskSequential.run();
  testOmpTaskSequential.post_processing();

  std::vector<double> parallel_res(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(parallel_res.data()));
  taskDataPar->outputs_count.emplace_back(parallel_res.size());

  TestTBBSequentialNedelinCannon testSTLTaskParallel(taskDataPar);
  ASSERT_EQ(testSTLTaskParallel.validation(), true);
  testSTLTaskParallel.pre_processing();
  testSTLTaskParallel.run();
  testSTLTaskParallel.post_processing();

  for (size_t i = 0; i < seq_block_result.size(); ++i) {
    ASSERT_EQ(parallel_res[i], seq_block_result[i]);
  }
}

TEST(nedelin_d_block_cannons_stl, Multiply_0x0) {
  int n = 0;
  int m = 0;

  std::vector<double> in_mtrx_A = RndMatrix(n, m);
  std::vector<double> in_mtrx_B = RndMatrix(n, m);
  std::vector<double> seq_block_result(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_block_result.data()));
  taskDataSeq->outputs_count.emplace_back(seq_block_result.size());

  TestTBBSequentialNedelinCannon testSTLTaskParallel(taskDataSeq);
  ASSERT_EQ(testSTLTaskParallel.validation(), true);
  testSTLTaskParallel.pre_processing();
  testSTLTaskParallel.run();
  testSTLTaskParallel.post_processing();

  std::vector<double> parallel_res(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(parallel_res.data()));
  taskDataPar->outputs_count.emplace_back(parallel_res.size());

  for (size_t i = 0; i < seq_block_result.size(); ++i) {
    ASSERT_EQ(parallel_res[i], seq_block_result[i]);
  }
}

TEST(nedelin_d_block_cannons_stl, Multiply_100x100) {
  int n = 100;
  int m = 100;

  std::vector<double> in_mtrx_A = RndMatrix(n, m);
  std::vector<double> in_mtrx_B = RndMatrix(n, m);
  std::vector<double> seq_block_result(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_block_result.data()));
  taskDataSeq->outputs_count.emplace_back(seq_block_result.size());

  TestTBBSequentialNedelinCannon testTBBTaskSequential(taskDataSeq);
  ASSERT_EQ(testTBBTaskSequential.validation(), true);
  testTBBTaskSequential.pre_processing();
  testTBBTaskSequential.run();
  testTBBTaskSequential.post_processing();

  std::vector<double> parallel_res(n * m);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_A.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_A.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_mtrx_B.data()));
  taskDataPar->inputs_count.emplace_back(in_mtrx_B.size());

  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&n));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(&m));

  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(parallel_res.data()));
  taskDataPar->outputs_count.emplace_back(parallel_res.size());

  TestTaskSTLParallelNedelinCannon testSTLTaskParallel(taskDataPar);
  ASSERT_EQ(testSTLTaskParallel.validation(), true);
  testSTLTaskParallel.pre_processing();
  testSTLTaskParallel.run();
  testSTLTaskParallel.post_processing();

  for (size_t i = 0; i < seq_block_result.size(); ++i) {
    ASSERT_EQ(parallel_res[i], seq_block_result[i]);
  }
}