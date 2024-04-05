// Copyright 2024 Platonova Maria
#include <gtest/gtest.h>

#include <vector>

#include "seq/platonova_m_jarvis_method/include/ops_seq.hpp"

TEST(platonova_m_jarvis_method, Empty_Input) {
  std::vector<Point> points = {};
  std::vector<Point> hull = {};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  TestTaskSequentialJarvis testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), false);
}

TEST(platonova_m_jarvis_method, Single_Point) {
  std::vector<Point> points = {{0, 0}};
  std::vector<Point> hull = {{0, 0}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  TestTaskSequentialJarvis testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  ASSERT_EQ(resHull[0], hull[0]);
}

TEST(platonova_m_jarvis_method, Two_Points) {
  std::vector<Point> points = {{0, 0}, {1, 1}};
  std::vector<Point> hull = {{0, 0}, {1, 1}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  TestTaskSequentialJarvis testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  ASSERT_EQ(resHull[0], hull[0]);
  ASSERT_EQ(resHull[1], hull[1]);
}

TEST(platonova_m_jarvis_method, Duplicate_Points) {
  std::vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {0, 0}, {1, 1}, {2, 2}};
  std::vector<Point> hull = {{0, 0}, {2, 2}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  TestTaskSequentialJarvis testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  ASSERT_EQ(resHull[0], hull[0]);
  ASSERT_EQ(resHull[1], hull[1]);
}

TEST(platonova_m_jarvis_method, Random_Points) {
  std::vector<Point> points = {{1, 4}, {3, 8}, {8, 2}, {5, 5}, {9, 1}, {4, 7}};
  std::vector<Point> hull = {{1, 4}, {3, 8}, {4, 7}, {9, 1}};
  std::vector<Point> resHull(hull.size());
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  TestTaskSequentialJarvis testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(resHull[i], hull[i]);
  }
}

TEST(platonova_m_jarvis_method, Rectangle_Points) {
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
  TestTaskSequentialJarvis testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(resHull[i], hull[i]);
  }
}
