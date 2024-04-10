// Copyright 2024 Shokurov Daniil
#include "stl/shokurov_d_convex_hull/include/ops_stl.hpp"

#include <cmath>
#include <functional>
#include <future>
#include <thread>
#include <utility>

using namespace std::chrono_literals;

namespace shokurov_d_convex_hull_stl {
bool ConvexHullShokurov::validation() {
  internal_order_test();
  try {
    if (taskData->inputs_count.size() != 1) throw 1;
    if (taskData->inputs_count[0] == 0) throw 2;
    if (taskData->outputs_count.size() != 1) throw 3;
    // if (taskData->outputs_count[0] != taskData->inputs_count[0]) throw 4;
    return true;
  } catch (...) {
    return false;
  }
}

bool ConvexHullShokurov::pre_processing() {
  internal_order_test();
  try {
    // Init value for input and output
    auto* input_ = reinterpret_cast<std::pair<double, double>*>(taskData->inputs[0]);
    size_t n = taskData->inputs_count[0];
    points.assign(input_, input_ + n);
    return true;
  } catch (...) {
    return false;
  }
}

bool ConvexHullShokurov::run() {
  internal_order_test();
  try {
    si = solve(points);
    return true;
  } catch (...) {
    return false;
  }
}

bool ConvexHullShokurov::post_processing() {
  internal_order_test();
  try {
    auto* outputs_ = reinterpret_cast<std::pair<double, double>*>(taskData->outputs[0]);
    for (size_t i = 0; i < si; ++i) {
      outputs_[i] = points[i];
    }
    taskData->outputs_count[0] = si;
    return true;
  } catch (...) {
    return false;
  }
}

template <class T>
size_t find_min(const T* arr, const size_t size, std::function<bool(const T& a, const T& b)> less) {
  size_t index = 0;
  for (size_t i = 1; i < size; ++i) {
    if (less(arr[i], arr[index])) index = i;
  }
  return index;
}

template <class T>
size_t parallel_find_min(const size_t num_threads, const T* arr, const size_t size,
                         std::function<bool(const T& a, const T& b)> less) {
  size_t count = (size + num_threads - 1) / num_threads;
  auto handle = std::async(std::launch::async, find_min<T>, arr, count, less);
  size_t index1 = 0;
  if (num_threads >= 2 && size - count > 0) {
    index1 = count + parallel_find_min(num_threads - 1, arr + count, size - count, less);
  }
  size_t index2 = handle.get();
  if (less(arr[index2], arr[index1])) index1 = index2;
  return index1;
}

size_t ConvexHullShokurov::solve(std::vector<std::pair<double, double>>& p) {
  const size_t n = p.size();
  std::pair<double, double> p0 = p[index_lowest_right_point(p)];
  std::pair<double, double> vec = {1.0, 0.0};
  std::pair<double, double> pk = p0;
  size_t k = 0;
  const auto* arr = p.data();
  do {
    size_t index = k;
    index += parallel_find_min<std::pair<double, double>>(
        4, &arr[k], n - k, [&](const std::pair<double, double>& a, const std::pair<double, double>& b) {
          return !comp(a, pk) && my_less(sub(a, pk), sub(b, pk), vec);
        });
    std::swap(p[index], p[k]);
    vec = sub(p[k], pk);
    pk = p[k];
    ++k;
  } while (!comp(pk, p0));

  return k;
}

bool ConvexHullShokurov::comp(const std::pair<double, double>& a, const std::pair<double, double>& b) {
  return normal(sub(a, b)) < 1e-6;
}

bool ConvexHullShokurov::my_less(const std::pair<double, double>& v1, const std::pair<double, double>& v2,
                                   const std::pair<double, double>& v) {
  const double cosa = cos(v1, v);
  const double cosb = cos(v2, v);
  bool flag = false;
  if (std::abs(cosa - cosb) > 1e-7)
    flag = cosa > cosb;
  else
    flag = normal(v1) > normal(v2);
  return flag;
}

std::pair<double, double> ConvexHullShokurov::sub(const std::pair<double, double>& v1,
                                                    const std::pair<double, double>& v2) {
  return std::pair<double, double>(v1.first - v2.first, v1.second - v2.second);
}

size_t ConvexHullShokurov::index_lowest_right_point(const std::vector<std::pair<double, double>>& v) {
  auto less = [](const std::pair<double, double>& a, const std::pair<double, double>& b) {
    bool flag = false;
    if (a.second != b.second)
      flag = a.second < b.second;
    else
      flag = a.first > b.first;
    return flag;
  };
  size_t index = 0;
  if (!v.empty()) {
    for (size_t i = 1; i < v.size(); ++i) {
      if (less(v[i], v[index])) {
        index = i;
      }
    }
  } else {
    throw 11;
  }
  return index;
}

double ConvexHullShokurov::scalar_product(const std::pair<double, double>& v1, const std::pair<double, double>& v2) {
  return v1.first * v2.first + v1.second * v2.second;
}

double ConvexHullShokurov::normal(const std::pair<double, double>& v) {
  return std::sqrt(v.first * v.first + v.second * v.second);
}

double ConvexHullShokurov::cos(const std::pair<double, double>& v1, const std::pair<double, double>& v2) {
  const double n1 = normal(v1);
  const double n2 = normal(v2);
  return scalar_product(v1, v2) / (n1 * n2);
}
}  // namespace shokurov_d_convex_hull_stl