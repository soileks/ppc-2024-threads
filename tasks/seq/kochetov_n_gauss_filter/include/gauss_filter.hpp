// Copyright 2024 Kochetov Nikolay
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class GaussFilter : public ppc::core::Task {
 public:
  explicit GaussFilter(std::shared_ptr<ppc::core::TaskData> taskData) : Task(std::move(taskData)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  int* input;
  int* result;
  int x, y;
  int* getImgId(int a, int b);
  int* getResId(int a, int b);
  int clampData(int value, int min, int max);
  std::vector<std::vector<double>> kernel = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
};
