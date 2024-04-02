// Copyright 2024 Kosarev Egor
#include <gtest/gtest.h>

#include <vector>

#include "omp/kosarev_e_jarvis/include/ops_omp.hpp"

TEST(Kosarev_e_jarvis_omp, Test_hull_one) {
  std::vector<Point> points = {{0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3}};
  std::vector<Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  TestTaskSequentialKosarevJarvisHull testOmpTaskSequential(taskDataSeq);
  ASSERT_EQ(testOmpTaskSequential.validation(), true);
  testOmpTaskSequential.pre_processing();
  testOmpTaskSequential.run();
  testOmpTaskSequential.post_processing();

  // Create data
  std::vector<Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  TestOMPTaskParallelKosarevJarvisHull testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_omp, Test_hull_two) {
  std::vector<Point> points = {{2, 5},  {7, 3},  {1, 1}, {-1, 4}, {-5, 4}, {-4, 1}, {-1, -2},
                               {2, -3}, {3, -1}, {2, 2}, {1, 5},  {-4, 5}, {-1, 0}};
  std::vector<Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  TestTaskSequentialKosarevJarvisHull testOmpTaskSequential(taskDataSeq);
  ASSERT_EQ(testOmpTaskSequential.validation(), true);
  testOmpTaskSequential.pre_processing();
  testOmpTaskSequential.run();
  testOmpTaskSequential.post_processing();

  // Create data
  std::vector<Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  TestOMPTaskParallelKosarevJarvisHull testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_omp, Test_hull_three) {
  std::vector<Point> points = {{2, 5}, {7, 3}, {1, 1}};
  std::vector<Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  TestTaskSequentialKosarevJarvisHull testOmpTaskSequential(taskDataSeq);
  ASSERT_EQ(testOmpTaskSequential.validation(), true);
  testOmpTaskSequential.pre_processing();
  testOmpTaskSequential.run();
  testOmpTaskSequential.post_processing();

  // Create data
  std::vector<Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  TestOMPTaskParallelKosarevJarvisHull testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_omp, Test_hull_no_hull) {
  std::vector<Point> points = {};
  std::vector<Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  TestTaskSequentialKosarevJarvisHull testOmpTaskSequential(taskDataSeq);
  ASSERT_EQ(testOmpTaskSequential.validation(), true);
  testOmpTaskSequential.pre_processing();
  testOmpTaskSequential.run();
  testOmpTaskSequential.post_processing();

  // Create data
  std::vector<Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  TestOMPTaskParallelKosarevJarvisHull testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_omp, Test_hull_five) {
  std::vector<Point> points = {{1, 4}, {1, 5}, {1, 6}, {1, 2}, {1, 3}, {2, 5}, {3, 4}, {0, 5}, {1, 7}};
  std::vector<Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  TestTaskSequentialKosarevJarvisHull testOmpTaskSequential(taskDataSeq);
  ASSERT_EQ(testOmpTaskSequential.validation(), true);
  testOmpTaskSequential.pre_processing();
  testOmpTaskSequential.run();
  testOmpTaskSequential.post_processing();

  // Create data
  std::vector<Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  TestOMPTaskParallelKosarevJarvisHull testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}