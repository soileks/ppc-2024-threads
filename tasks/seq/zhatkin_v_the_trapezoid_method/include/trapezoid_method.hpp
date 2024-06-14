// Copyright 2024 Zhatkin Vyacheslav
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

class ZhatkinTaskSequential : public ppc::core::Task {
 public:
  explicit ZhatkinTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_,
                                 std::function<double(double, double)> func)
      : Task(std::move(taskData_)), f(std::move(func)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::function<double(double, double)> f;
  double lowerx{}, upperx{}, lowery{}, uppery{}, res = 0.0;
  int nx{}, ny{};
};

double trapezoidal_integral(const std::function<double(double, double)>& f, double lowerx, double upperx, double lowery,
                            double uppery, int nx, int ny);
