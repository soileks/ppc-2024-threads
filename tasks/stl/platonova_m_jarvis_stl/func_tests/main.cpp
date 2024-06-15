// Copyright 2024 Platonova Mariya
#include <gtest/gtest.h>

#include <vector>

#include "stl/platonova_m_jarvis_stl/include/ops_stl.hpp"

TEST(Platonova_m_jarvis, can_operate_empty) {
  std::vector<Point> points = {};
  std::vector<Point> res(points.size());

  // Create data
  std::vector<Point> resParallel(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resParallel.data()));
  taskDataPar->outputs_count.emplace_back(resParallel.size());

  // Create Task
  JarvisSTL TaskSTL(taskDataPar);
  ASSERT_EQ(TaskSTL.validation(), false);
}

TEST(Platonova_m_jarvis, can_operate_line) {
  std::vector<Point> points = {{0, 0}, {0, 1}};
  std::vector<Point> hull = {{0, 0}, {0, 1}};
  std::vector<Point> res(hull.size());

  // CreateTaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataOmp->inputs_count.emplace_back(points.size());
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskDataOmp->outputs_count.emplace_back(res.size());

  // Create TBB Task
  JarvisSTL TaskSTL(taskDataOmp);
  ASSERT_EQ(TaskSTL.validation(), true);
  TaskSTL.pre_processing();
  TaskSTL.run();
  TaskSTL.post_processing();

  for (size_t i = 0; i < hull.size(); ++i) {
    ASSERT_EQ(res[i], hull[i]);
  }
}

TEST(Platonova_m_jarvis, can_operate_one_point) {
  std::vector<Point> points = {{0, 0}};
  std::vector<Point> hull = {{0, 0}};
  std::vector<Point> resHull(hull.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataTbb->inputs_count.emplace_back(points.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull.data()));
  taskDataTbb->outputs_count.emplace_back(resHull.size());

  // Create Task
  JarvisSTL TaskSTL(taskDataTbb);
  ASSERT_EQ(TaskSTL.validation(), true);
  TaskSTL.pre_processing();
  TaskSTL.run();
  TaskSTL.post_processing();

  ASSERT_EQ(resHull[0], hull[0]);
}

TEST(Platonova_m_jarvis, diagonal_points) {
  std::vector<Point> points = {{0, 0}, {1, 1}};
  std::vector<Point> expected_hull = points;

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskData->inputs_count.emplace_back(points.size());
  std::vector<Point> res(points.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskData->outputs_count.emplace_back(res.size());
  JarvisSTL TaskSTL(taskData);

  // Проверяем валидность входных данных
  ASSERT_EQ(TaskSTL.validation(), true);
  TaskSTL.pre_processing();
  TaskSTL.run();
  TaskSTL.post_processing();

  for (size_t i = 0; i < expected_hull.size(); ++i) {
    ASSERT_EQ(res[i], expected_hull[i]);
  }
}

TEST(Platonova_m_jarvis, diagonal_points2) {
  std::vector<Point> points = {{1, 1}, {2, 2}};
  std::vector<Point> expected_hull = points;

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskData->inputs_count.emplace_back(points.size());
  std::vector<Point> res(points.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(res.data()));
  taskData->outputs_count.emplace_back(res.size());
  JarvisSTL TaskSTL(taskData);

  // Проверяем валидность входных данных
  ASSERT_EQ(TaskSTL.validation(), true);
  TaskSTL.pre_processing();
  TaskSTL.run();
  TaskSTL.post_processing();

  for (size_t i = 0; i < expected_hull.size(); ++i) {
    ASSERT_EQ(res[i], expected_hull[i]);
  }
}