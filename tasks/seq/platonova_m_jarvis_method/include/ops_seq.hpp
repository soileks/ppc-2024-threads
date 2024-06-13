// Copyright 2024 Platonova Maria
#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

struct Point {
  int x, y;

  bool operator==(const Point& other) const { return x == other.x && y == other.y; }
  bool operator<(const Point& other) const {
    if (x != other.x) {
      return x < other.x;
    }
    return y < other.y;
  }
};

class JarvisSeq : public ppc::core::Task {
 public:
  explicit JarvisSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<Point> points;
  std::vector<Point> convexHullPoints;
};

std::vector<Point> Jarvis(const std::vector<Point>& points);