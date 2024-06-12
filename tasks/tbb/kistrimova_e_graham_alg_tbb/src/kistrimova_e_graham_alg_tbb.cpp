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
  tbb::parallel_for(0, n, 1, [&](int i) { R[i] = i; });

  int min_x_idx = tbb::parallel_reduce(
      tbb::blocked_range<int>(1, n), 0,
      [&](const tbb::blocked_range<int>& range, int init) {
        for (int i = range.begin(); i != range.end(); ++i) {
          if (points[R[i]].x < points[R[init]].x) {
            init = i;
          }
        }
        return init;
      },
      [](int idx1, int idx2) {
        return (points[R[idx1]].x < points[R[idx2]].x ||
                (points[R[idx1]].x == points[R[idx2]].x && points[R[idx1]].y < points[R[idx2]].y))
                   ? idx1
                   : idx2;
      });
  std::swap(R[0], R[min_x_idx]);

  tbb::parallel_sort(R.begin() + 1, R.end(),
                     [&](int a, int b) { return rotate(points[R[0]], points[a], points[b]) > 0; });

  std::vector<point> res{points[R[0]], points[R[1]]};
  for (int i = 2; i < n; i++) {
    while (rotate(res.end()[-2], res.end()[-1], points[R[i]]) <= 0) {
      res.pop_back();
    }
    res.push_back(points[R[i]]);
  }

  return res;
}