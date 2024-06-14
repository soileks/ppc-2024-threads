// Copyright 2024 Kistrimova Ekaterina
#include "seq/kistrimova_e_graham_alg/include/ops_seq.hpp"

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
  for (int i = 0; i < n; i++) R[i] = i;

  for (int i = 1; i < n; i++) {
    if (points[R[i]].x < points[R[0]].x) std::swap(R[i], R[0]);
  }

  for (int i = 2; i < n; i++) {
    int j = i;
    while (j > 1 && rotate(points[R[0]], points[R[j - 1]], points[R[j]]) < 0) {
      std::swap(R[j], R[j - 1]);
      j--;
    }
  }

  std::vector<point> res{points[R[0]], points[R[1]]};
  for (int i = 2; i < n; i++) {
    while (rotate(res.end()[-2], res.end()[-1], points[R[i]]) <= 0) res.pop_back();
    res.push_back(points[R[i]]);
  }

  return res;
}