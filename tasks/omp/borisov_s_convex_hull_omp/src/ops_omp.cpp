// Copyright 2024 Borisov Saveliy
#include "omp/borisov_s_convex_hull_omp/include/ops_omp.hpp"

#include <thread>

using namespace std::chrono_literals;

namespace BorisovSaveliyOMP {
bool ConvexHull::pre_processing() {
  internal_order_test();
  // Init value for input and output
  input = taskData->inputs[0];
  output = taskData->outputs[0];
  width = taskData->inputs_count[0];
  height = taskData->inputs_count[1];
  image.resize(width * height);
  std::memcpy(image.data(), input, width * height);
  points = convertToPoints(image, height, width);
  return true;
}

bool ConvexHull::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] >= 3 && taskData->inputs_count[1] >= 3 &&
         taskData->outputs_count[0] == taskData->inputs_count[0] &&
         taskData->outputs_count[1] == taskData->inputs_count[1];
}

bool ConvexHull::run() {
  internal_order_test();
  convexHullImage();
  return true;
}

bool ConvexHull::post_processing() {
  internal_order_test();
  std::memcpy(output, image.data(), width * height);
  return true;
}

int ConvexHull::isLeft(const Point& p1, const Point& p2, const Point& point) {
  return ((p2.x - p1.x) * (point.y - p1.y) - (point.x - p1.x) * (p2.y - p1.y));
}

bool ConvexHull::isCollinear(const Point& p1, const Point& p2, const Point& p3) {
  return (p2.y - p1.y) * (p3.x - p2.x) == (p3.y - p2.y) * (p2.x - p1.x);
}

bool ConvexHull::isOnSegment(const Point& p1, const Point& p2, const Point& point) {
  return (point.x <= std::max(p1.x, p2.x) && point.x >= std::min(p1.x, p2.x) && point.y <= std::max(p1.y, p2.y) &&
          point.y >= std::min(p1.y, p2.y) && isCollinear(p1, p2, point));
}

int ConvexHull::windingNumber(const std::vector<Point>& polygon, const Point& point) {
  int wn = 0;

  for (size_t i = 0; i < polygon.size(); ++i) {
    const Point& p1 = polygon[i];
    const Point& p2 = polygon[(i + 1) % polygon.size()];

    if (p1.y <= point.y) {
      if (p2.y > point.y && isLeft(p1, p2, point) > 0) {
        ++wn;
      }
    } else {
      if (p2.y <= point.y && isLeft(p1, p2, point) < 0) {
        --wn;
      }
    }
  }
  return wn;
}

bool ConvexHull::isInside(const std::vector<Point>& convexHull, const Point& point) {
  for (size_t i = 0; i < convexHull.size(); ++i) {
    const Point& p1 = convexHull[i];
    const Point& p2 = convexHull[(i + 1) % convexHull.size()];

    if (isOnSegment(p1, p2, point)) {
      return true;
    }
  }
  return windingNumber(convexHull, point) != 0;
}

bool ConvexHull::pointIsToTheRight(const Point& previous, const Point& current, const Point& potential) {
  int vectorCP_x = previous.x - current.x;
  int vectorCP_y = previous.y - current.y;
  int vectorCPotential_x = potential.x - current.x;
  int vectorCPotential_y = potential.y - current.y;

  int crossProduct = vectorCP_x * vectorCPotential_y - vectorCP_y * vectorCPotential_x;

  return crossProduct > 0;
}

std::vector<Point> ConvexHull::convertToPoints(const std::vector<uint8_t>& _image, int _height, int _width) {
  std::vector<Point> _points;

  for (int i = 0; i < _height; ++i) {
    for (int j = 0; j < _width; ++j) {
      if (_image[i * _width + j] == 1) {
        _points.emplace_back(i, j);
      }
    }
  }

  return _points;
}

std::vector<int> ConvexHull::convertToImageVector(const std::vector<Point>& _points, int _height, int _width) {
  std::vector<int> imageVector(height * width, 0);

#pragma omp parallel for
  for (const Point& point : _points) {
    int index = point.x * _width + point.y;
    if (index < _height * _width) {
      image[index] = 1;
    }
  }

  return imageVector;
}

#pragma omp declare reduction(merge : std::vector<Point> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

void ConvexHull::convexHullImage() {
  size_t psize = points.size();
  if (psize < 3) {
    return;
  }

  std::vector<Point> remainingPoints(points);
  std::vector<Point> convexHull;
  size_t startIndex = 0;
#pragma omp parallel for reduction(min : startIndex)
  for (size_t i = 0; i < remainingPoints.size(); i++) {
    if (remainingPoints[startIndex].x > remainingPoints[i].x ||
        ((remainingPoints[startIndex].x == remainingPoints[i].x) &&
         (remainingPoints[startIndex].y > remainingPoints[i].y))) {
      startIndex = i;
    }
  }

  Point startingPoint = remainingPoints[startIndex];
  convexHull.push_back(startingPoint);
  Point nextPoint;
  do {
    nextPoint = remainingPoints[0];
    if (nextPoint == convexHull.back()) {
      nextPoint = remainingPoints[1];
    }
#pragma omp parallel for shared(remainingPoints, convexHull, nextPoint)
    for (size_t i = 0; i < remainingPoints.size(); i++) {
      if ((remainingPoints[i] == convexHull.back()) || (convexHull.back() == nextPoint)) {
        continue;
      }
#pragma omp critical
      {
        if ((remainingPoints[i] == nextPoint) || pointIsToTheRight(convexHull.back(), nextPoint, remainingPoints[i])) {
          nextPoint = remainingPoints[i];
        }
      }
    }
    convexHull.push_back(nextPoint);
    if (convexHull.size() == points.size()) {
      break;
    }
  } while (convexHull.back() != startingPoint);

  std::vector<Point> localNewPoints;
  int i, j;

  std::vector<Point> copy(convexHull.begin(), convexHull.end());

#pragma omp parallel private(localNewPoints, i, j) shared(convexHull)
  {
#pragma omp for nowait
    for (i = 0; i < height; ++i) {
      for (j = 0; j < width; ++j) {
#pragma omp critical
        {
          if (isInside(copy, Point(i, j))) {
            localNewPoints.emplace_back(i, j);
          }
        }
      }
    }

#pragma omp critical
    convexHull.insert(convexHull.end(), localNewPoints.begin(), localNewPoints.end());
  }

  convertToImageVector(convexHull, height, width);
}
}  // namespace BorisovSaveliyOMP
