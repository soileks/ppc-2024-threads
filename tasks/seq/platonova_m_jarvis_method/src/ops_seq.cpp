// Copyright 2024 Platonova Maria
#include "seq/platonova_m_jarvis_method/include/ops_seq.hpp"

#include <algorithm>
#include <cmath>
#include <thread>
#include <vector>
#include <unordered_set>
#include <limits>

using namespace std::chrono_literals;

int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? -1 : 1;
}

std::vector<Point> Jarvis(const std::vector<Point>& points) {
  int n = points.size();
  if (n < 3) return {};
  std::vector<Point> hull;
  int l = 0;
  for (int i = 1; i < n; i++) {
    if (points[i].x < points[l].x) {
      l = i;
    }
  }

  int p = l, q;
  do {
    hull.push_back(points[p]);
    q = (p + 1) % n;
    for (int i = 0; i < n; i++) {
        if (orientation(points[p], points[i], points[q]) == 1)
            q = i;
    }
    p = q;
  } while (p != l);

  return hull;
}

bool JarvisSeq::pre_processing() {
  internal_order_test();
  // Init value for input and output
  points.resize(taskData->inputs_count[0]);
  auto* tmp_ptr_A = reinterpret_cast<Point*>(taskData->inputs[0]);
  std::copy_n(tmp_ptr_A, taskData->inputs_count[0], points.begin());
  return true;
}

bool JarvisSeq::validation() {
  internal_order_test();
  if (taskData->inputs_count[0] == 0) {
    return false;
  }

  std::sort(points.begin(), points.end());
  return std::unique(points.begin(), points.end()) == points.end();
}

bool JarvisSeq::run() {
  internal_order_test();
  convexHullPoints = Jarvis(points);
  return true;
}

bool JarvisSeq::post_processing() {
  internal_order_test();
  auto* output_ptr = reinterpret_cast<Point*>(taskData->outputs[0]);
  std::copy_n(convexHullPoints.begin(), convexHullPoints.size(), output_ptr);
  return true;
}