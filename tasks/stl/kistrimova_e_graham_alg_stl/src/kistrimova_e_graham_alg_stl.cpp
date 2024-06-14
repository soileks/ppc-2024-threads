// Copyright 2024 Kistrimova Ekaterina
#include <algorithm>
#include <cmath>
#include <mutex>
#include <thread>

#include "stl/kistrimova_e_graham_alg_stl/include/ops_stl.hpp"

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

double rotate(point X, point Y, point Z) { return (Y.x - X.x) * (Z.y - Y.y) - (Y.y - X.y) * (Z.x - Y.x); }

void compute_angles(std::vector<point>& points, std::vector<int>& R, int start, int end, const point& p0,
                    std::vector<double>& angles) {
  for (int i = start; i < end; ++i) {
    double dx = points[R[i]].x - p0.x;
    double dy = points[R[i]].y - p0.y;
    angles[i] = std::atan2(dy, dx);
  }
}

std::vector<point> graham(std::vector<point> points) {
  int n = points.size();
  std::vector<int> R(n);
  for (int i = 0; i < n; i++) R[i] = i;

  auto p0_iter = std::min_element(points.begin(), points.end(), [](const point& a, const point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
  });
  std::swap(points[0], *p0_iter);
  std::swap(R[0], R[std::distance(points.begin(), p0_iter)]);

  std::vector<double> angles(n);
  int num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(num_threads);
  int chunk_size = n / num_threads;

  for (int i = 0; i < num_threads; ++i) {
    int start = i * chunk_size;
    int end = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
    threads[i] = std::thread(compute_angles, std::ref(points), std::ref(R), start, end, points[0], std::ref(angles));
  }

  for (auto& t : threads) {
    t.join();
  }

  std::sort(R.begin() + 1, R.end(), [&angles](int i, int j) { return angles[i] < angles[j]; });
  std::vector<point> res{points[R[0]], points[R[1]]};
  for (int i = 2; i < n; i++) {
    while (rotate(res.end()[-2], res.end()[-1], points[R[i]]) <= 0) {
      res.pop_back();
    }
    res.push_back(points[R[i]]);
  }

  return res;
}