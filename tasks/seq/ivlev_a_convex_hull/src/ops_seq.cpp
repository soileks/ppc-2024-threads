// Copyright 2024 Ivlev Alexander
#include "seq/ivlev_a_convex_hull/include/ops_seq.hpp"

#include <thread>

using namespace std::chrono_literals;

bool ConvexHullSequential::pre_processing() {
  internal_order_test();
  try {
    components = reinterpret_cast<std::pair<size_t, size_t>>(taskData->inputs[0]);
    image.resize(taskData->inputs_count[0]);
    results = reinterpret_cast<std::pair<size_t, size_t>>(taskData->outputs[0]);
  } catch(...) {
    return false;
  }
  return true;
}

bool ConvexHullSequential::validation() {
  internal_order_test();

  return taskData->inputs_count[0] == 1 && taskData->outputs_count[0] == 1;
}

bool ConvexHullSequential::run() {
  internal_order_test();
  try {
    image[0] = ToImage(components[0], (2, 2));
    results[0] = ToComponents(image[0], (2, 2));
  } catch(...) {
    return false;
  }
  return true;
}

bool ConvexHullSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<int*>(taskData->outputs[0])[0] = results[0];
  return true;
}

std::vector<std::pair<size_t, size_t>> ConvexHullSequential::ToComponents(const std::vector<int>& image_, std::pair<size_t, size_t> size_) {
  std::vector<std::pair<size_t, size_t>> res;

  for (size_t i = 0; i < image_.size(), i++) {
    if (image_[i] == 1) {
      res.emplace_back(i/size_.second, i%size_.second);
    }
  }

  return res;
}

std::vector<int> ConvexHullSequential::ToImage(const std::vector<std::pair<size_t, size_t>>& component_, std::pair<size_t, size_t> size_) {
  size_t height = size_.first;
  size_t width = size_.second;
  std::vector<int> res(height * width, 0);

  for (const std::pair<size_t, size_t>& point : component_) {
      image[point.first * width + point.second] = 1;
  }

  return res;
}
