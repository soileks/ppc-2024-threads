// Copyright 2024 Akopyan Zal
#include "stl/akopyan_z_bin_hull/include/ops_stl.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <stack>
#include <thread>
#include <utility>
#include <vector>

namespace task = akopyan_z_bin_hull_stl;

bool task::AkopyanZBinHull::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] > 0 && taskData->inputs_count[1] > 0;
}

bool task::AkopyanZBinHull::pre_processing() {
  internal_order_test();
  width = taskData->inputs_count[0];
  height = taskData->inputs_count[1];
  auto* input = reinterpret_cast<int*>(taskData->inputs[0]);
  bin_image = std::vector<int>(input, input + width * height);
  out_hull = {};
  return true;
}

bool task::AkopyanZBinHull::run() {
  internal_order_test();
  for (const auto& component : label_components()) {
    for (auto p : graham(component)) {
      out_hull.emplace_back(p.x);
      out_hull.emplace_back(p.y);
    }
    out_hull.emplace_back(SEPARATOR);
  }
  return true;
}

bool task::AkopyanZBinHull::post_processing() {
  internal_order_test();
  std::copy(out_hull.begin(), out_hull.end(), reinterpret_cast<int*>(taskData->outputs[0]));
  return true;
}

std::vector<std::vector<task::pnt>> task::AkopyanZBinHull::label_components() {
  std::vector<std::vector<task::pnt>> comps;
  std::vector<std::vector<std::vector<task::pnt>>> thread_comp_points;
  std::vector<std::thread> threads;

  const int VISITED_BASE = 42;
  int num_threads = 4;
  int sz = (height + num_threads - 1) / num_threads;
  int left = sz * num_threads - height;
  thread_comp_points.resize(num_threads, {});

  auto worker = [&](int thred_num) {
    int loc_sz = thred_num == num_threads - 1 ? sz - left : sz;
    const int visited = VISITED_BASE + thred_num;

    for (int i_loc = thred_num * sz; i_loc < (thred_num + 1) * sz && i_loc < height; i_loc += sz) {
      for (int i = i_loc; i < i_loc + loc_sz; ++i) {
        for (int j = 0; j < width; ++j) {
          if (bin_image[i * width + j] == 1) {
            std::vector<task::pnt> comp{};
            comp.emplace_back(j, i);
            const int loc_visited = visited + thread_comp_points[thred_num].size() * num_threads;
            bin_image[i * width + j] = loc_visited;

            std::stack<task::pnt> st;
            st.emplace(j, i);

            while (!st.empty()) {
              int x = st.top().x;
              int y = st.top().y;
              st.pop();

              for (int dx = 0; dx <= 1; dx++) {
                for (int dy = 0; dy <= 1; dy++) {
                  int nx = x + dx;
                  int ny = y + dy;
                  if (0 <= nx && nx < width && i_loc <= ny && ny < i_loc + loc_sz && bin_image[ny * width + nx] == 1) {
                    comp.emplace_back(nx, ny);
                    bin_image[ny * width + nx] = loc_visited;
                    st.emplace(nx, ny);
                  }
                }
              }
            }
            thread_comp_points[thred_num].emplace_back(comp);
          }
        }
      }
    }
  };

  threads.reserve(num_threads);
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(worker, i);
  }

  for (auto& t : threads) {
    t.join();
  }

  for (size_t th = 0; th < thread_comp_points.size() - 1; ++th) {
    auto& thread_comps = thread_comp_points[th];
    int th_b = th + 1;
    int edge_b = sz * th_b;

    for (int j = 0; j < width; ++j) {
      if (bin_image[(edge_b - 1) * width + j] == 0) continue;

      for (int dx = -1; dx <= 1; ++dx) {
        int j_dx = j + dx;
        if (j_dx < 0 || j_dx >= width || bin_image[edge_b * width + j_dx] == 0) continue;

        int visit_u = bin_image[(edge_b - 1) * width + j] - VISITED_BASE;
        int visit_b = bin_image[edge_b * width + j_dx] - VISITED_BASE;
        size_t comp_id_u = (visit_u - th) / num_threads;
        size_t comp_id_b = (visit_b - th_b) / num_threads;

        if (comp_id_u < thread_comp_points[th].size() && !thread_comp_points[th_b][comp_id_b].empty()) {
          thread_comp_points[th][comp_id_u].insert(thread_comp_points[th][comp_id_u].end(),
                                                   std::make_move_iterator(thread_comp_points[th_b][comp_id_b].begin()),
                                                   std::make_move_iterator(thread_comp_points[th_b][comp_id_b].end()));
          thread_comp_points[th_b][comp_id_b].clear();
        }
      }
    }

    for (auto& comp : thread_comps) {
      if (!comp.empty()) {
        comps.emplace_back(std::move(comp));
      }
    }
    thread_comps.clear();
  }

  auto& thread_comps_last = thread_comp_points.back();
  for (auto& comp : thread_comps_last) {
    if (!comp.empty()) {
      comps.emplace_back(std::move(comp));
    }
  }
  thread_comps_last.clear();

  return comps;
}

inline int rotate(task::pnt a, task::pnt b, task::pnt c) {
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}
inline double dist(task::pnt a, task::pnt b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); }
inline double det(task::pnt a, task::pnt b, task::pnt c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool cmp(task::pnt left_point, task::pnt a, task::pnt b) {
  const double det_ = det(left_point, a, b);
  if (det_ == 0) return dist(left_point, a) < dist(left_point, b);
  return det_ > 0;
}

std::vector<task::pnt> task::AkopyanZBinHull::graham(std::vector<task::pnt> points) {
  if (points.size() < 3) return points;

  int left_index = 0;
  task::pnt left_point = points[left_index];

  for (size_t i = 1; i < points.size(); i++) {
    if (points[i].x < left_point.x || (points[i].x == left_point.x && points[i].y < left_point.y)) {
      left_point = points[i];
      left_index = i;
    }
  }

  std::swap(points[left_index], points[points.size() - 1]);
  points.pop_back();

  std::sort(points.begin(), points.end(),
            [left_point](const task::pnt& a, const task::pnt& b) { return cmp(left_point, a, b); });

  std::vector<task::pnt> hull = {left_point, points[0]};
  for (size_t i = 1; i < points.size(); i++) {
    const auto sz = hull.size();
    const auto c = points[i];

    const double rotate_ = rotate(hull[sz - 2], hull[sz - 1], c);
    if (rotate_ == 0) {
      hull[sz - 1] = c;
    } else {
      if (rotate_ < 0) {
        while (rotate(hull[hull.size() - 2], hull[hull.size() - 1], c) <= 0) {
          hull.pop_back();
        }
      }
      hull.push_back(c);
    }
  }

  return hull;
}
