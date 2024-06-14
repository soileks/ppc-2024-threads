// Copyright 2024 Kachalov Mikhail
#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

using integ_func = double (*)(double, double);

class IntegralOMPMonteCarlo : public ppc::core::Task {
 public:
  explicit IntegralOMPMonteCarlo(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  integ_func function{};
  double Int1[2]{}, Int2[2]{};

  int N{};
  double res{};
};

class IntegralSequentialMonteCarlo : public ppc::core::Task {
 public:
  explicit IntegralSequentialMonteCarlo(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  integ_func function{};
  double Int1[2]{}, Int2[2]{};

  int N{};
  double res{};
};
