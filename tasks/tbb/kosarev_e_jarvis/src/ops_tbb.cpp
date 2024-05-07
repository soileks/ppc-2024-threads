// Copyright 2024 Kosarev Egor
#include "tbb/kosarev_e_jarvis/include/ops_tbb.hpp"


namespace Kosarev_e_tbb_KosarevJarvisHull {

int orientation(const Point& p, const Point& q, const Point& r) {
  int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  if (val == 0) return 0;    // collinear
  return (val > 0) ? 1 : 2;  // clock or counterclock wise
}

Point generateRandomPoint(int minX, int maxX, int minY, int maxY) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<int> disX(minX, maxX);
  std::uniform_int_distribution<int> disY(minY, maxY);
  return {disX(gen), disY(gen)};
}

std::vector<Point> generateRandomPoints(int numPoints, int minX, int maxX, int minY, int maxY) {
  std::vector<Point> points;
  points.reserve(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    points.push_back(generateRandomPoint(minX, maxX, minY, maxY));
  }
  return points;
}

double distance(const Point& p1, const Point& p2) {
  return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

std::vector<Point> JarvisAlgo(const std::vector<Point>& arrPoints) {
  if (arrPoints.size() < 3) return arrPoints;

  auto startPoint = *std::min_element(arrPoints.begin(), arrPoints.end(), [](const Point& p, const Point& q) {
    return p.y < q.y || (p.y == q.y && p.x < q.x);
  });

  std::vector<Point> convexHull;
  convexHull.push_back(startPoint);

  Point prevPoint = startPoint;

  while (true) {
    Point nextPoint = arrPoints[0];
    for (const auto& point : arrPoints) {
      if (point == prevPoint) continue;
      int orient = orientation(prevPoint, nextPoint, point);
      if (orient == 2 || (orient == 0 && distance(prevPoint, point) > distance(prevPoint, nextPoint))) {
        nextPoint = point;
      }
    }

    if (nextPoint == startPoint) break;
    convexHull.push_back(nextPoint);
    prevPoint = nextPoint;
  }

  return convexHull;
}

std::vector<Point> kosarev_tbb_jarvis(const std::vector<Point>& points) {
  size_t numberOfPoints = points.size();
  if (numberOfPoints < 3) return points;
  Point start = tbb::parallel_reduce(
      tbb::blocked_range<size_t>(1, numberOfPoints), points[0],
      [&points](const tbb::blocked_range<size_t>& r, Point localStart) -> Point {
        auto begin = r.begin();
        auto end = r.end();
        for (auto i = begin; i != end; i++) {
          if (points[i] < localStart) localStart = points[i];
        }
        return localStart;
      },
      [](const Point& localStart, const Point& start) -> Point { return localStart < start ? localStart : start; });
  std::vector<Point> result;
  result.push_back(start);
  Point currentPoint = start;
  Point nextPoint;

  do {
    if (currentPoint == points[0]) {
      nextPoint = points[1];
    } else {
      nextPoint = points[0];
    }
    currentPoint = tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, numberOfPoints), nextPoint,
        [&currentPoint, &points](const tbb::blocked_range<size_t>& r, Point localNext) -> Point {
          auto begin = r.begin();
          auto end = r.end();
          for (auto i = begin; i != end; i++) {
            int direction = (points[i].y - currentPoint.y) * (localNext.x - currentPoint.x) -
                            (points[i].x - currentPoint.x) * (localNext.y - currentPoint.y);
            if (direction > 0 ||
                (direction == 0 && ((points[i].x - currentPoint.x) * (points[i].x - currentPoint.x) +
                                    (points[i].y - currentPoint.y) * (points[i].y - currentPoint.y)) >
                                       ((localNext.x - currentPoint.x) * (localNext.x - currentPoint.x) +
                                        (localNext.y - currentPoint.y) * (localNext.y - currentPoint.y)))) {
              localNext = points[i];
            }
          }
          return localNext;
        },
        [&currentPoint](const Point& nextPoint, const Point& localNext) -> Point {
          int direction = (localNext.y - currentPoint.y) * (nextPoint.x - currentPoint.x) -
                          (localNext.x - currentPoint.x) * (nextPoint.y - currentPoint.y);
          if (direction > 0 ||
              (direction == 0 && ((localNext.x - currentPoint.x) * (localNext.x - currentPoint.x) +
                                  (localNext.y - currentPoint.y) * (localNext.y - currentPoint.y)) >
                                     ((nextPoint.x - currentPoint.x) * (nextPoint.x - currentPoint.x) +
                                      (nextPoint.y - currentPoint.y) * (nextPoint.y - currentPoint.y)))) {
            return localNext;
          }
          return nextPoint;
        });

    nextPoint = currentPoint;
    result.push_back(nextPoint);
  } while (currentPoint != start);

  result.pop_back();
  return result;
}
bool TestTaskTbbKosarevJarvisHull::pre_processing() {
  internal_order_test();
  points.resize(taskData->inputs_count[0]);
  auto* tmp_ptr_A = reinterpret_cast<Point*>(taskData->inputs[0]);
  std::copy_n(tmp_ptr_A, taskData->inputs_count[0], points.begin());
  return true;
}

bool TestTaskTbbKosarevJarvisHull::validation() {
  internal_order_test();
  if (taskData->inputs_count[0] == 0) {
    return false;
  }
  std::vector<Point> expectedConvexHull = kosarev_tbb_jarvis(points);
  std::sort(expectedConvexHull.begin(), expectedConvexHull.end());
  return std::unique(expectedConvexHull.begin(), expectedConvexHull.end()) == expectedConvexHull.end();
}

bool TestTaskTbbKosarevJarvisHull::run() {
  internal_order_test();
  convexHullPoints = kosarev_tbb_jarvis(points);
  return true;
}

bool TestTaskTbbKosarevJarvisHull::post_processing() {
  internal_order_test();
  auto* output_ptr = reinterpret_cast<Point*>(taskData->outputs[0]);
  std::copy_n(convexHullPoints.begin(), convexHullPoints.size(), output_ptr);
  return true;
}

bool TestTaskSequentialKosarevJarvisHull::pre_processing() {
  internal_order_test();
  // Init value for input and output
  points = std::vector<Point>(taskData->inputs_count[0]);

  auto* tmp_ptr_A = reinterpret_cast<Point*>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; ++i) {
    points[i] = tmp_ptr_A[i];
  }
  return true;
}

bool TestTaskSequentialKosarevJarvisHull::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] >= taskData->outputs_count[0];
}

bool TestTaskSequentialKosarevJarvisHull::run() {
  internal_order_test();
  pointsHull = JarvisAlgo(points);
  return true;
}
bool TestTaskSequentialKosarevJarvisHull::post_processing() {
  internal_order_test();
  std::copy(pointsHull.begin(), pointsHull.end(), reinterpret_cast<Point*>(taskData->outputs[0]));
  return true;
}

}  // namespace Kosarev_e_tbb_KosarevJarvisHull