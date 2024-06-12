// Copyright 2024 Kistrimova Ekaterina
#include <tbb/tbb.h>

#include "tbb/kistrimova_e_graham_alg_tbb/include/ops_tbb.hpp"

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

std::vector<point> graham(std::vector<point> points) {
  int n = points.size();
  std::vector<int> R(n);
  tbb::parallel_for(0, n, 1, [&R](int i) { R[i] = i; });

  tbb::parallel_for(1, n, 1, [&R, &points](int i) {
    if (points[R[i]].x < points[R[0]].x) {
      tbb::mutex mtx;
      tbb::mutex::scoped_lock lock(mtx);
      std::swap(R[i], R[0]);
    }
  });

  std::sort(R.begin() + 1, R.end(), [&points](int a, int b) { return rotate(points[0], points[a], points[b]) > 0; });

  std::vector<point> res{points[R[0]], points[R[1]]};
  tbb::parallel_for(2, n, 1, [&res, &R, &points](int i) {
    tbb::mutex mtx;
    tbb::mutex::scoped_lock lock(mtx);
    while (rotate(res[res.size() - 2], res[res.size() - 1], points[R[i]]) < 0 && res.size() > 1) {
      res.pop_back();
    }
    res.push_back(points[R[i]]);
  });

  return res;
}