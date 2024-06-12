// Copyright 2024 Platonova Mariya
#include "stl/platonova_m_jarvis_stl/include/ops_stl.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

std::vector<Point> Jarvis(const std::vector<Point>& points) {
  if (points.size() < 3) return points;

  auto minPoint = *std::min_element(points.begin(), points.end());
  std::vector<Point> convexHull = {minPoint};
  Point prevPoint = minPoint, nextPoint;

  auto findNextPoint = [](const Point& currentPoint, const std::vector<Point>& points, int start, int end, Point& candidate) {
    for (int i = start; i < end; ++i) {
      const auto& point = points[i];
      if (point == currentPoint) continue;
      double crossProduct = (point.y - currentPoint.y) * (candidate.x - currentPoint.x) - (point.x - currentPoint.x) * (candidate.y - currentPoint.y);
      double distCurrentPoint = std::pow(point.x - currentPoint.x, 2) + std::pow(point.y - currentPoint.y, 2);
      double distCandidate = std::pow(candidate.x - currentPoint.x, 2) + std::pow(candidate.y - currentPoint.y, 2);
      if (crossProduct > 0 || (crossProduct == 0 && distCurrentPoint > distCandidate)) candidate = point;
    }
  };

  do {
    nextPoint = points[0];
    int numThreads = std::thread::hardware_concurrency();
    int chunkSize = points.size() / numThreads;
    std::vector<std::thread> threads;
    std::vector<Point> candidates(numThreads, nextPoint);

    for (int i = 0; i < numThreads; ++i) {
      int start = i * chunkSize;
      int end = (i == numThreads - 1) ? points.size() : (i + 1) * chunkSize;
      threads.emplace_back(findNextPoint, std::ref(prevPoint), std::cref(points), start, end, std::ref(candidates[i]));
    }

    for (auto& thread : threads) {
      if (thread.joinable()) thread.join();
    }

    for (const auto& candidate : candidates) {
      double crossProduct = (candidate.y - prevPoint.y) * (nextPoint.x - prevPoint.x) - (candidate.x - prevPoint.x) * (nextPoint.y - prevPoint.y);
      double distPrevPoint = std::pow(candidate.x - prevPoint.x, 2) + std::pow(candidate.y - prevPoint.y, 2);
      double distNextPoint = std::pow(nextPoint.x - prevPoint.x, 2) + std::pow(nextPoint.y - prevPoint.y, 2);
      if (crossProduct > 0 || (crossProduct == 0 && distPrevPoint > distNextPoint)) nextPoint = candidate;
    }

    convexHull.push_back(nextPoint);
    prevPoint = nextPoint;
  } while (nextPoint != minPoint);

  return convexHull;
}

bool JarvisSTL::pre_processing() {
  internal_order_test();
  points.resize(taskData->inputs_count[0]);
  std::copy_n(reinterpret_cast<Point*>(taskData->inputs[0]), taskData->inputs_count[0], points.begin());
  return true;
}

bool JarvisSTL::validation() {
  internal_order_test();
  if (taskData->inputs_count[0] == 0) {
    return false;
  }
  std::sort(points.begin(), points.end());
  return std::unique(points.begin(), points.end()) == points.end();
}

bool JarvisSTL::run() {
  internal_order_test();
  resPoints = Jarvis(points);
  return true;
}

bool JarvisSTL::post_processing() {
  internal_order_test();
  std::copy_n(resPoints.begin(), resPoints.size(), reinterpret_cast<Point*>(taskData->outputs[0]));
  return true;
}
