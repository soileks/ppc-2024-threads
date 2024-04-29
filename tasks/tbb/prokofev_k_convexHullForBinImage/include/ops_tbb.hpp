// Copyright 2024 Prokofev Kirill
#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace prokofev_k_convexHull_Tbb {

std::vector<int> FindComponents(const std::vector<int>& image, int width, int height);
void FloodFill(std::vector<int>* image, int height, int width, int yStart, int xStart, int label);
int FindCountComponents(const std::vector<int>& image);
int FindCountPointsInComponent(const std::vector<int>& image);
std::vector<int> RemoveExtraPoints(const std::vector<int>& image, int width, int height, int label);
void Sort(std::vector<int>* points, int xMin, int yMin);
std::vector<int> GrahamAlgorithm(std::vector<int> points);
std::vector<int> ConvertImageToVector(std::vector<std::vector<int>> notVecImg, int width, int height);
std::vector<int> GenerateImgForPerfTests(int componentCount);
int Cross(int x1, int y1, int x2, int y2, int x3, int y3);

struct Point {
  int x, y;
  Point(int x, int y) : x(x), y(y) {}
};

class BinaryImageConvexHullTbb : public ppc::core::Task {
 public:
  explicit BinaryImageConvexHullTbb(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> img{};
  std::vector<int> res{};
  int width, height, resSize;
};
}  // namespace prokofev_k_convexHull_Tbb