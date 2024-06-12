// Copyright 2024 Kistrimova Ekaterina
#include <omp.h>

#include "omp/kistrimova_e_graham_alg_omp/include/ops_omp.hpp"

bool GrahamAlgTask::pre_processing() {
  internal_order_test();
  input = std::vector<point>(taskData->inputs_count[0]);
  auto* in = reinterpret_cast<point*>(taskData->inputs[0]);
  for (unsigned int i = 0; i < taskData->inputs_count[0]; i++) {
    input[i] = in[i];
  }
  output = input;
  return true;
}

bool GrahamAlgTask::validation() {
  internal_order_test();
  return taskData->outputs_count[0] <= taskData->inputs_count[0];
}

bool GrahamAlgTask::run() {
  internal_order_test();
  output = graham(input);
  return true;
}

bool GrahamAlgTask::post_processing() {
  internal_order_test();
  std::copy(output.begin(), output.end(), reinterpret_cast<point*>(taskData->outputs[0]));
  return true;
}

double rotate(point X, point Y, point Z) {
  return (Y.x - X.x) * (Z.y - Y.y) - (Y.y - X.y) * (Z.x - Y.x);
}

bool compare(point a, point b, point base) {
  double rot = rotate(base, a, b);
  if (rot == 0)
    return (a.x - base.x) * (a.x - base.x) + (a.y - base.y) * (a.y - base.y) <
           (b.x - base.x) * (b.x - base.x) + (b.y - base.y) * (b.y - base.y);
  return rot > 0;
}

std::vector<point> graham(std::vector<point> points) {
  int n = points.size();
  if (n < 3) return points;

  int min_x_idx = 0;
#pragma omp parallel for reduction(min:min_x_idx)
  for (int i = 1; i < n; ++i) {
    if (points[i].x < points[min_x_idx].x) {
      min_x_idx = i;
    }
  }
  std::swap(points[0], points[min_x_idx]);

  point base = points[0];
  std::sort(points.begin() + 1, points.end(), [base](point a, point b) { return compare(a, b, base); });

  std::vector<point> hull;
  hull.push_back(points[0]);
  hull.push_back(points[1]);

#pragma omp parallel
  {
    std::vector<point> private_hull;
#pragma omp for nowait
    for (int i = 2; i < n; ++i) {
      while (private_hull.size() > 1 && rotate(private_hull[private_hull.size() - 2], private_hull.back(), points[i]) <= 0) { private_hull.pop_back(); }
      private_hull.push_back(points[i]);
    }

#pragma omp critical
    hull.insert(hull.end(), private_hull.begin(), private_hull.end());
  }

  std::sort(hull.begin(), hull.end(), [base](point a, point b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });
  hull.erase(unique(hull.begin(), hull.end(), [](point a, point b) { return a.x == b.x && a.y == b.y; }), hull.end());

  return hull;
}