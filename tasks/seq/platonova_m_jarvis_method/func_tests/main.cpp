// Copyright 2024 Platonova Maria
#include <gtest/gtest.h>

#include <vector>

#include "seq/platonova_m_jarvis_method/include/ops_seq.hpp"

TEST(platonova_m_jarvis_method, can_operate_empty) {
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
  JarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), false);
}

TEST(platonova_m_jarvis_method, can_operate_two_point) {
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
  JarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  ASSERT_EQ(resHull[0], hull[0]);
}

TEST(platonova_m_jarvis_method, diagonal_points) {
  std::vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}};
  std::vector<Point> hull = points;
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  JarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(resHull[0], hull[0]);
  ASSERT_EQ(resHull[1], hull[1]);
}

TEST(platonova_m_jarvis_method, diagonal_points2) {
  std::vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}};
  std::vector<Point> hull = points;
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  JarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(resHull[0], hull[0]);
  ASSERT_EQ(resHull[1], hull[1]);
}

TEST(platonova_m_jarvis_method, Rectangle_Points) {
  std::vector<Point> points = {{0, 0}, {0, 5}, {5, 5}, {5, 0}};
  std::vector<Point> hull = {{0, 0}, {5, 0}, {5, 5}, {0, 5}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataSeq->outputs_count.emplace_back(resHull.size());

  // Create Task
  JarvisSeq testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();

  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(resHull[i], hull[i]);
  }
}
