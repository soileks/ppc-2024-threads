// Copyright 2024 Platonova Mariya
#include "omp/platonova_m_jarvis_omp/include/ops_omp.hpp"

#include <omp.h>

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

std::vector<Point> Jarvis(const std::vector<Point>& Points) {
  if (Points.size() < 3) return Points;
  Point p0 = *std::min_element(Points.begin(), Points.end(), [](Point a, Point b){ return a.x < b.x || (a.x == b.x && a.y < b.y); });
  std::vector<Point> convexHull = {p0};
  Point prevPoint = p0, nextPoint;
  do {
    nextPoint = Points[0];
    for (const auto& point : Points) {
      if (point == prevPoint) continue;
      double crossProduct = (point.y - prevPoint.y) * (nextPoint.x - prevPoint.x) - (point.x - prevPoint.x) * (nextPoint.y - prevPoint.y);
      double distPrevPoint = pow(point.x - prevPoint.x, 2) + pow(point.y - prevPoint.y, 2);
      double distNextPoint = pow(nextPoint.x - prevPoint.x, 2) + pow(nextPoint.y - prevPoint.y, 2);
      if (crossProduct > 0 || (crossProduct == 0 && distPrevPoint > distNextPoint)) nextPoint = point;
    }
    convexHull.push_back(nextPoint);
    prevPoint = nextPoint;
  } while (nextPoint != p0);
  return convexHull;
}

std::vector<Point> Jarvis_omp(const std::vector<Point>& Points, int num_threads) {
  std::vector<Point> result;
  omp_set_num_threads(num_threads);
  int delta = Points.size() / num_threads, remains = Points.size() % num_threads;
#pragma omp parallel
  {
    int threadNum = omp_get_thread_num();
    int localSize = (threadNum == 0) ? remains + delta : Points.size() / num_threads;
    std::vector<Point> local(Points.begin() + (localSize * threadNum) + (threadNum == 0 ? 0 : remains), Points.begin() + (localSize * (threadNum + 1)) + (threadNum == 0 ? 0 : remains));
#pragma omp critical
    result.insert(result.end(), Jarvis(local).begin(), Jarvis(local).end());
  }
  return Jarvis(result);
}

bool TestOMPJarvisSeq::pre_processing() {
  internal_order_test();
  points.resize(taskData->inputs_count[0]);
  auto* tmp_ptr = reinterpret_cast<Point*>(taskData->inputs[0]);
  std::copy_n(tmp_ptr, taskData->inputs_count[0], points.begin());
  return true;
}

bool TestOMPJarvisSeq::validation() {
  internal_order_test();
  if (taskData->inputs_count[0] == 0) {
    return false;
  }
  std::sort(points.begin(), points.end());
  return std::unique(points.begin(), points.end()) == points.end();
}

bool TestOMPJarvisSeq::run() {
  internal_order_test();
  resPoints = Jarvis(points);
  return true;
}

bool TestOMPJarvisSeq::post_processing() {
  internal_order_test();
  auto* output_ptr = reinterpret_cast<Point*>(taskData->outputs[0]);
  std::copy_n(resPoints.begin(), resPoints.size(), output_ptr);
  return true;
}

bool TestOMPJarvisParallel::pre_processing() {
  internal_order_test();
  points.resize(taskData->inputs_count[0]);
  auto* tmp_ptr = reinterpret_cast<Point*>(taskData->inputs[0]);
  std::copy_n(tmp_ptr, taskData->inputs_count[0], points.begin());
  return true;
}

bool TestOMPJarvisParallel::validation() {
  internal_order_test();
  if (taskData->inputs_count[0] == 0) {
    return false;
  }
  std::sort(points.begin(), points.end());
  return std::unique(points.begin(), points.end()) == points.end();
}

bool TestOMPJarvisParallel::run() {
   internal_order_test();
  resPoints = Jarvis_omp(points, 4);
  return true;
}

bool TestOMPJarvisParallel::post_processing() {
  internal_order_test();
  auto* output_ptr = reinterpret_cast<Point*>(taskData->outputs[0]);
  std::copy_n(resPoints.begin(), resPoints.size(), output_ptr);
  return true;
}
