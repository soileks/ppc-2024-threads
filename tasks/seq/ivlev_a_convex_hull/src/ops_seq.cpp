// Copyright 2024 Ivlev Alexander
#include "seq/ivlev_a_convex_hull/include/ops_seq.hpp"

#include <thread>

using namespace std::chrono_literals;

bool ConvexHullSequential::pre_processing() {
  internal_order_test();
  try {
    size_t n = taskData->inputs_count[0];
    components.resize(n);
    for (size_t i = 0; i < n; i++) {
      auto* input_ = reinterpret_cast<std::pair<size_t, size_t> *>(taskData->inputs[i]);
      size_t tmp_size = taskData->inputs_count[i + 1];
      components[i].assign(input_, input_ + tmp_size);
      size_t m_w = 0;

      for (size_t j = 0; j < tmp_size; j++) {
        if (components[i][j].second > m_w) {
          m_w = components[i][j].second;
        }
      }
      std::sort(components[i].begin(), components[i].end());
      sizes.emplace_back(components[i].back().first + 1, m_w + 1);
    }
    images.resize(taskData->inputs_count[0]);
    results.resize(taskData->inputs_count[0]);
  } catch (...) {
    std::cout << "pre\n";
    return false;
  }
  return true;
}

bool ConvexHullSequential::validation() {
  internal_order_test();
  try {
    if (taskData->inputs_count.size() <= 1) return false;
    if (taskData->outputs_count.size() <= 0) return false;
    if (taskData->inputs_count[0] < 1) return false;
    for (size_t i = 0; i < taskData->inputs_count[0]; i++) {
      if (taskData->inputs[i] == nullptr) return false;
      if (taskData->inputs_count[i] < 1) return false;
    }
    if (taskData->outputs[0] == nullptr) return false;

    // m.b. dop check
  } catch (...) {
    std::cout << "val\n";
    return false;
  }

  return true;
}

bool ConvexHullSequential::run() {
  internal_order_test();
  try {
    for (size_t i = 0; i <taskData->inputs_count[0]; i++) {
      //images[i] = ToImage(components[i], sizes[i]);
      //results[i] = ToComponents(images[i], sizes[i]);
      results[i] = Convex_Hull(components[i]);
    }
  } catch (...) {
    std::cout << "run\n";
    return false;
  }
  return true;
}

bool ConvexHullSequential::post_processing() {
  internal_order_test();
  try {
    size_t n = taskData->inputs_count[0];
    auto* outputs_ = reinterpret_cast<std::vector<std::pair<size_t, size_t>> *>(taskData->outputs[0]);
    for (size_t i = 0; i < n; i++) {
      std::sort(results[i].begin(), results[i].end());
      outputs_[i].clear();
      for (size_t j = 0; j < results[i].size(); j++) {
        outputs_[i].push_back(results[i][j]);
      }
    }
  } catch (...) {
    std::cout << "post\n";
    return false;
  }
  return true;
}

std::vector<std::pair<size_t, size_t>> ConvexHullSequential::ToComponents(const std::vector<size_t>& image_,
                                                                          std::pair<size_t, size_t> size_) {
  std::vector<std::pair<size_t, size_t>> res;

  for (size_t i = 0; i < image_.size(); i++) {
    if (image_[i] == 1) {
      res.emplace_back(i / size_.second, i % size_.second);
    }
  }

  return res;
}

std::vector<size_t> ConvexHullSequential::ToImage(const std::vector<std::pair<size_t, size_t>>& component_,
                                               std::pair<size_t, size_t> size_) {
  size_t height = size_.first;
  size_t width = size_.second;
  std::vector<size_t> res(height * width, 0);

  for (const std::pair<size_t, size_t>& point : component_) {
    res[point.first * width + point.second] = 1;
  }

  return res;
}

size_t rotation(const std::pair<int, int>& a, const std::pair<int, int>& b, const std::pair<int, int>& c)
{
    int tmp = (b.first - a.first) * (c.second - b.second) - (b.second - a.second) * (c.first - b.first);
    if (tmp == 0) return 0;
    return ((tmp > 0) ? 1 : 2);
}

std::vector<std::pair<size_t, size_t>> ConvexHullSequential::Convex_Hull(const std::vector<std::pair<size_t, size_t>>& component_)
{
    size_t n = component_.size();
    if (n < 3) return component_;

    size_t left = 0;
    for (size_t i = 1; i < n; i++)
        if (component_[i].second < component_[left].second)
            left = i;

    std::vector<std::pair<size_t, size_t>> res = {};
    size_t p = left, q;
    do
    {
        q = (p + 1) % n;
        for (size_t i = 0; i < n; i++)
        {
          if (rotation(component_[p], component_[i], component_[q]) == 2 || 
          (rotation(component_[p], component_[i], component_[q]) == 0 && i > p && i != q)) {
            q = i;
          }
        }
        res.push_back(component_[q]);
        p = q;
    }
    while (p != left);

    return res;
}
