// Copyright 2024 Kriseev Mikhail
#include "stl/kriseev_m_convex_hull_graham/include/kriseev_m_convex_hull_stl.hpp"

#include <algorithm>
#include <barrier>
#include <limits>
#include <thread>

namespace KriseevMTaskStl {

double angle1(const KriseevMTaskStl::Point &origin, const KriseevMTaskStl::Point &point) {
  double dx = point.first - origin.first;
  double dy = point.second - origin.second;
  if (dx == 0.0 && dy == 0.0) {
    // Return this incorrect value to ensure the origin is always first in sorted array
    return -1.0;
  }
  // Calculating angle using algorithm from https://stackoverflow.com/a/14675998/14116050
  return (dx >= 0 ? dy / (dx + dy) : 1 - dx / (dy - dx));
}
bool compareForSort1(const KriseevMTaskStl::Point &origin, const KriseevMTaskStl::Point &a,
                     const KriseevMTaskStl::Point &b) {
  double angleA = angle1(origin, a);
  double angleB = angle1(origin, b);
  if (angleA < angleB) {
    return true;
  }
  if (angleA > angleB) {
    return false;
  }
  double dxA = a.first - origin.first;
  double dyA = a.second - origin.second;
  double dxB = b.first - origin.first;
  double dyB = b.second - origin.second;
  // The further point must be first
  // so the others will be ignored
  return dxA * dxA + dyA * dyA > dxB * dxB + dyB * dyB;
}

void sortPoints(std::vector<Point> &points, const KriseevMTaskStl::Point &origin) {
  size_t numThreads = std::thread::hardware_concurrency();

  size_t chunkSize = points.size() / numThreads;
  if (points.size() % numThreads > 0) {
    chunkSize++;
  }
  if (points.size() <= numThreads) {
    chunkSize = points.size();
    numThreads = 1;
  }

  std::vector<std::thread> threads;
  std::barrier b(numThreads);

  auto work = [&b](std::vector<Point> &points, Point origin, size_t chunkSize, size_t threadNum) {
    size_t firstIndex = threadNum * chunkSize;
    if ((firstIndex & 1) == 0) firstIndex++;

    size_t lastIndex = (threadNum + 1) * chunkSize;
    if (lastIndex > points.size()) {
      lastIndex = points.size();
    }
    size_t pointsSize = points.size();
    auto pointsBegin = points.begin();
    // std::cout << "thread " << threadNum << " work " << firstIndex << " to " << lastIndex << "\n";

    size_t lastIndexOdd = lastIndex == points.size() ? lastIndex - 1 : lastIndex;
    for (size_t phase = 0; phase < pointsSize; phase++) {
      if ((phase & 1) == 0) {
        for (size_t i = firstIndex; i < lastIndex; i += 2) {
          if (compareForSort1(origin, points[i], points[i - 1])) {
            std::iter_swap(pointsBegin + i, pointsBegin + i - 1);
          }
        }
      } else {
        for (size_t i = firstIndex; i < lastIndexOdd; i += 2) {
          if (compareForSort1(origin, points[i + 1], points[i])) {
            std::iter_swap(pointsBegin + i, pointsBegin + i + 1);
          }
        }
      }

      b.arrive_and_wait();
    }
  };
  for (size_t t = 0; t < numThreads; ++t) {
    threads.emplace_back(work, std::ref(points), origin, chunkSize, t);
  }
  for (size_t t = 0; t < numThreads; ++t) {
    threads[t].join();
  }
}

bool checkOrientation(const KriseevMTaskStl::Point &origin, const KriseevMTaskStl::Point &a,
                      const KriseevMTaskStl::Point &b) {
  double dxA = a.first - origin.first;
  double dyA = a.second - origin.second;
  double dxB = b.first - origin.first;
  double dyB = b.second - origin.second;
  // Using cross product to check orientation
  return dxA * dyB - dyA * dxB < 0;
}

bool KriseevMTaskStl::ConvexHullTask::pre_processing() {
  internal_order_test();
  auto *pointsX = reinterpret_cast<double *>(taskData->inputs.at(0));
  auto *pointsY = reinterpret_cast<double *>(taskData->inputs.at(1));
  points = std::vector<std::pair<double, double>>();
  points.reserve(taskData->inputs_count[0]);
  for (size_t i = 0; i < taskData->inputs_count[0]; ++i) {
    points.emplace_back(pointsX[i], pointsY[i]);
  }

  return true;
}

bool KriseevMTaskStl::ConvexHullTask::validation() {
  internal_order_test();
  if (taskData->inputs_count.at(0) != taskData->inputs_count.at(1)) {
    return false;
  }
  if (taskData->inputs_count[0] < 3) {
    return false;
  }
  return true;
}

bool KriseevMTaskStl::ConvexHullTask::run() {
  internal_order_test();
  auto originIt =
      std::min_element(points.begin(), points.end(), [](auto &a, auto &b) -> bool { return a.second < b.second; });
  auto origin = *originIt;

  sortPoints(points, origin);

  std::vector<Point> hull;
  hull.reserve(points.size());
  hull.push_back(points[0]);
  hull.push_back(points[1]);
  hull.push_back(points[2]);

  for (uint32_t i = 3; i < points.size(); ++i) {
    while (hull.size() > 1 && !checkOrientation(hull.back(), *(hull.end() - 2), points[i])) {
      hull.pop_back();
    }
    hull.push_back(points[i]);
  }
  this->finalHull = hull;
  return true;
}
bool KriseevMTaskStl::ConvexHullTask::post_processing() {
  internal_order_test();
  auto *resultsX = reinterpret_cast<double *>(taskData->outputs[0]);
  auto *resultsY = reinterpret_cast<double *>(taskData->outputs[1]);
  for (size_t i = 0; i < finalHull.size(); ++i) {
    resultsX[i] = finalHull[i].first;
    resultsY[i] = finalHull[i].second;
  }
  taskData->outputs_count[0] = finalHull.size();
  taskData->outputs_count[1] = finalHull.size();
  return true;
}

}  // namespace KriseevMTaskStl
