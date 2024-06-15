// Copyright 2024 Fedotov Kirill
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

double func_x_plus_y(double num1, double num2);

namespace fedotov_stl {

class FedotovTaskSeq : public ppc::core::Task {
 public:
  explicit FedotovTaskSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double (*function)(double, double){};
  double Coordinates_For_Integration1[2], Coordinates_For_Integration2[2];

  int part_of_integrate;
  double integration_result;
};
}  // namespace fedotov_stl