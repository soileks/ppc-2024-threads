// Copyright 2024 Lapin Dmitriy
#pragma once
#include <vector>

#include "core/task/include/task.hpp"

class BlockFilterSeq : public ppc::core::Task {
public:
  explicit BlockFilterSeq(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

private:
  int width;
  int height;
  std::vector<std::vector<int>> *mas_in;
  std::vector<std::vector<int>> *mas_out;
};