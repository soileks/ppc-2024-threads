// Copyright 2024 Tushentsova Karina
#pragma once

#include <list>
#include <random>
#include <thread>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

class MarkingInageOmp : public ppc::core::Task {
 public:
  explicit MarkingInageOmp(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  int height{}, width{};
  std::vector<std::vector<uint32_t>> sourse = {};
  std::vector<std::vector<uint32_t>> destination = {};
};