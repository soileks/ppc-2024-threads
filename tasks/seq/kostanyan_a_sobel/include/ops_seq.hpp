// Copyright 2024 Kostanyan Arsen
#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

std::vector<uint8_t> kostanyan_getRandomPicture(int n, int m, uint8_t min, uint8_t max);

class kostanyan_EdgeDetectionSequential : public ppc::core::Task {
 public:
  explicit kostanyan_EdgeDetectionSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
   : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> input_ = {};
  std::vector<uint8_t> res = {};
  int n{}, m{};
};
