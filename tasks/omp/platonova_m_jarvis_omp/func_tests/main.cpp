// Copyright 2024 Platonova Mariya
#include <gtest/gtest.h>

#include <vector>

#include "omp/platonova_m_jarvis_omp/include/ops_omp.hpp"

TEST(Platonova_m_jarvis, can_operate_empty) {
  std::vector<Point> points = {};
  std::vector<Point> res(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataSeq->outputs_count.emplace_back(res.size());

  // Create Task
  TestOMPJarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), false);

  // Create data
  std::vector<Point> resParallel(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resParallel.data()));
  taskDataPar->outputs_count.emplace_back(resParallel.size());

  // Create Task
  TestOMPJarvisParallel testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), false);
}

TEST(Platonova_m_jarvis, can_operate_random_points) {
  std::vector<Point> points = {{2, 5}, {4, 9}, {9, 3}, {6, 6}, {10, 2}, {5, 8}};
  std::vector<Point> hull = {{2, 5}, {4, 9}, {5, 8}, {9, 3}, {10, 2}};
  std::vector<Point> res(hull.size());
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataSeq->outputs_count.emplace_back(res.size());

  // Create Task
  TestOMPJarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create OMP TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataOmp->inputs_count.emplace_back(points.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataOmp->outputs_count.emplace_back(res.size());

  // Create OMP Task
  TestOMPJarvisParallel testOmpTaskParallel(taskDataOmp);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(res[i], hull[i]);
  }
}

TEST(Platonova_m_jarvis, can_operate_one_point) {
  std::vector<Point> points = {{0, 0}};
  std::vector<Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  TestOMPJarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create data
  std::vector<Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  TestOMPJarvisParallel testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Platonova_m_jarvis, can_operate_two_points) {
  std::vector<Point> points = {{0, 0}, {1, 1}};
  std::vector<Point> resHull_seq(points.size());

  // Create TaskData for sequential version
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task for sequential version
  TestOMPJarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create data for parallel version
  std::vector<Point> resHull_par(points.size());

  // Create TaskData for parallel version
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task for parallel version
  TestOMPJarvisParallel testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Platonova_m_jarvis, can_operate_repeat_points) {
  std::vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {0, 0}, {1, 1}, {2, 2}};
  std::vector<Point> resHull_seq(2);

  // Create TaskData for sequential version
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task for sequential version
  TestOMPJarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create data for parallel version
  std::vector<Point> resHull_par(2);

  // Create TaskData for parallel version
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task for parallel version
  TestOMPJarvisParallel testOmpTaskParallel(taskDataPar);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Platonova_m_jarvis, test4) {
  std::vector<Point> points = {{0, 0}, {0, 5}, {5, 5}, {5, 0}, {1, 1}, {1, 4}, {4, 4}, {4, 1}};
  std::vector<Point> hull = {{0, 0}, {0, 5}, {5, 5}, {5, 0}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  TestOMPJarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  // Create OMP TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataOmp->inputs_count.emplace_back(points.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataOmp->outputs_count.emplace_back(resHull.size());

  // Create OMP Task
  TestOMPJarvisParallel testOmpTaskParallel(taskDataOmp);
  ASSERT_EQ(testOmpTaskParallel.validation(), true);
  testOmpTaskParallel.pre_processing();
  testOmpTaskParallel.run();
  testOmpTaskParallel.post_processing();

  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(resHull[i], hull[i]);
  }
}
