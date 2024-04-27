// Copyright 2024 Soloninko Andrey
#include <gtest/gtest.h>

#include <vector>

#include "omp/soloninko_a_radix_int_batcher/include/ops_omp.hpp"

TEST(soloninko_a_Test_Vec_OMP, Test_Vec_20_omp) {
  // Create data
  std::vector<int> vec = sol_GetRandVector(20);
  std::vector<int> seq_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_res.data()));
  taskDataSeq->outputs_count.emplace_back(seq_res.size());

  // Create Task
  TaskOMPSequentialBatcherSoloninko testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  std::vector<int> omp_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(omp_res.data()));
  taskDataPar->outputs_count.emplace_back(omp_res.size());

  // Create Task
  TestTaskOMPParallelBatcherSoloninko testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < vec.size(); i++) {
    ASSERT_EQ(seq_res[i], omp_res[i]);
  }
}

TEST(soloninko_a_Test_Vec_OMP, Test_Vec_40_omp) {
  // Create data
  std::vector<int> vec = sol_GetRandVector(40);
  std::vector<int> seq_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_res.data()));
  taskDataSeq->outputs_count.emplace_back(seq_res.size());

  // Create Task
  TaskOMPSequentialBatcherSoloninko testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  std::vector<int> omp_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(omp_res.data()));
  taskDataPar->outputs_count.emplace_back(omp_res.size());

  // Create Task
  TestTaskOMPParallelBatcherSoloninko testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < vec.size(); i++) {
    ASSERT_EQ(seq_res[i], omp_res[i]);
  }
}

TEST(soloninko_a_Test_Vec_OMP, Test_Vec_60_omp) {
  // Create data
  std::vector<int> vec = sol_GetRandVector(60);
  std::vector<int> seq_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_res.data()));
  taskDataSeq->outputs_count.emplace_back(seq_res.size());

  // Create Task
  TaskOMPSequentialBatcherSoloninko testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  std::vector<int> omp_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(omp_res.data()));
  taskDataPar->outputs_count.emplace_back(omp_res.size());

  // Create Task
  TestTaskOMPParallelBatcherSoloninko testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < vec.size(); i++) {
    ASSERT_EQ(seq_res[i], omp_res[i]);
  }
}

TEST(soloninko_a_Test_Vec_OMP, Test_Vec_80_omp) {
  // Create data
  std::vector<int> vec = sol_GetRandVector(80);
  std::vector<int> seq_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_res.data()));
  taskDataSeq->outputs_count.emplace_back(seq_res.size());

  // Create Task
  TaskOMPSequentialBatcherSoloninko testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  std::vector<int> omp_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(omp_res.data()));
  taskDataPar->outputs_count.emplace_back(omp_res.size());

  // Create Task
  TestTaskOMPParallelBatcherSoloninko testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < vec.size(); i++) {
    ASSERT_EQ(seq_res[i], omp_res[i]);
  }
}

TEST(soloninko_a_Test_Vec_OMP, Test_Vec_100_omp) {
  // Create data
  std::vector<int> vec = sol_GetRandVector(100);
  std::vector<int> seq_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(seq_res.data()));
  taskDataSeq->outputs_count.emplace_back(seq_res.size());

  // Create Task
  TaskOMPSequentialBatcherSoloninko testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  std::vector<int> omp_res(vec.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(omp_res.data()));
  taskDataPar->outputs_count.emplace_back(omp_res.size());

  // Create Task
  TestTaskOMPParallelBatcherSoloninko testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < vec.size(); i++) {
    ASSERT_EQ(seq_res[i], omp_res[i]);
  }
}
