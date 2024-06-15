// Copyright 2024 Prokofev Kirill
#include <gtest/gtest.h>

#include <vector>

#include "tbb/prokofev_k_convexHullForBinImage/include/ops_tbb.hpp"

TEST(prokofev_k_convex_hull_tbb, BinarImgConvexHullTest1) {
  // Create data
  std::vector<int> out(22);
  std::vector<std::vector<int>> notVecImage({{0, 1, 0, 0, 0, 1, 1, 1},
                                             {1, 1, 0, 0, 1, 1, 1, 1},
                                             {1, 1, 0, 1, 1, 1, 1, 1},
                                             {1, 0, 0, 1, 0, 0, 0, 1},
                                             {1, 1, 0, 0, 0, 0, 1, 1}});
  int width = 8;
  int height = 5;
  std::vector<int> vecImage = prokofev_k_convexHull_Tbb::ConvertImageToVector(notVecImage, width, height);
  std::vector<int> trueRes = {0, 1, 1, 0, 1, 4, 0, 4, -1, 3, 2, 4, 1, 7, 0, 7, 4, 6, 4, 3, 3, -1};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(out.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (size_t i = 0; i < trueRes.size(); i++) {
    ASSERT_EQ(trueRes[i], out[i]);
  }
}

TEST(prokofev_k_convex_hull_tbb, BinarImgConvexHullTest2) {
  // Create data
  std::vector<int> out(25);
  std::vector<std::vector<int>> notVecImage({{1, 1, 1, 1, 1, 1, 1, 1},
                                             {1, 0, 0, 0, 0, 0, 0, 1},
                                             {1, 0, 1, 0, 1, 1, 0, 1},
                                             {1, 0, 1, 0, 0, 0, 0, 1},
                                             {1, 0, 0, 1, 0, 1, 0, 1},
                                             {1, 0, 1, 0, 1, 1, 0, 1},
                                             {1, 0, 0, 0, 0, 0, 0, 1},
                                             {1, 1, 1, 1, 1, 1, 1, 1}});
  int width = 8;
  int height = 8;
  std::vector<int> vecImage = prokofev_k_convexHull_Tbb::ConvertImageToVector(notVecImage, width, height);
  std::vector<int> trueRes = {0, 0, 7, 0, 7, 7, 0, 7, -1, 2, 2, 5, 4, 5, 5, 4, 5, 2, 5, -1, 4, 2, 5, 2, -1};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(out.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (size_t i = 0; i < trueRes.size(); i++) {
    ASSERT_EQ(trueRes[i], out[i]);
  }
}

TEST(prokofev_k_convex_hull_tbb, BinarImgConvexHullTest3) {
  // Create data
  std::vector<int> out(18);
  std::vector<std::vector<int>> notVecImage({{1, 1, 0, 0, 0, 0, 1, 1},
                                             {1, 1, 0, 0, 0, 0, 1, 1},
                                             {1, 1, 0, 0, 0, 0, 1, 1},
                                             {1, 1, 0, 0, 0, 0, 1, 1},
                                             {1, 1, 0, 0, 0, 0, 1, 1}});
  int width = 8;
  int height = 5;
  std::vector<int> vecImage = prokofev_k_convexHull_Tbb::ConvertImageToVector(notVecImage, width, height);
  std::vector<int> trueRes = {0, 0, 1, 0, 1, 4, 0, 4, -1, 6, 0, 7, 0, 7, 4, 6, 4, -1};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(out.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (size_t i = 0; i < trueRes.size(); i++) {
    ASSERT_EQ(trueRes[i], out[i]);
  }
}
TEST(prokofev_k_convex_hull_tbb, BinarImgConvexHullTest4) {
  // Create data
  std::vector<int> out(9);
  std::vector<std::vector<int>> notVecImage({{0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 0, 0, 0, 1, 0, 0, 0, 0},
                                             {0, 0, 0, 1, 0, 1, 0, 0, 0},
                                             {0, 0, 1, 0, 0, 0, 1, 0, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0}});
  int width = 9;
  int height = 5;
  std::vector<int> vecImage = prokofev_k_convexHull_Tbb::ConvertImageToVector(notVecImage, width, height);
  std::vector<int> trueRes = {2, 3, 3, 2, 5, 2, 6, 3, -1};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(out.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (size_t i = 0; i < trueRes.size(); i++) {
    ASSERT_EQ(trueRes[i], out[i]);
  }
}

TEST(prokofev_k_convex_hull_tbb, BinarImgConvexHullTest5) {
  // Create data
  std::vector<int> out(9);
  std::vector<std::vector<int>> notVecImage({{0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0},
                                             {0, 0, 1, 1, 1, 1, 0, 0}});
  int width = 8;
  int height = 10;
  std::vector<int> vecImage = prokofev_k_convexHull_Tbb::ConvertImageToVector(notVecImage, width, height);
  std::vector<int> trueRes = {2, 0, 5, 0, 5, 9, 2, 9, -1};
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(vecImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->inputs_count.emplace_back(out.size());
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  // Create Task
  prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb testTaskTbb(taskDataTbb);
  ASSERT_EQ(testTaskTbb.validation(), true);
  testTaskTbb.pre_processing();
  testTaskTbb.run();
  testTaskTbb.post_processing();
  for (size_t i = 0; i < trueRes.size(); i++) {
    ASSERT_EQ(trueRes[i], out[i]);
  }
}