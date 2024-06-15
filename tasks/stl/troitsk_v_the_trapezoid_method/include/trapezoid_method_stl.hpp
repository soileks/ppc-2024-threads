// Copyright 2024 Troitskiy Alexandr
#pragma once

#include <cmath>
#include <functional>
#include <future>
#include <memory>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

class troitskTaskTBBSequential : public ppc::core::Task {
 public:
  explicit troitskTaskTBBSequential(std::shared_ptr<ppc::core::TaskData> taskData_,
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

double trapezoidal_integralTR(const std::function<double(double, double)>& f, double lowerx, double upperx,
                              double lowery, double uppery, int nx, int ny);

class troitskTaskSTL : public ppc::core::Task {
 public:
  explicit troitskTaskSTL(std::shared_ptr<ppc::core::TaskData> taskData_, std::function<double(double, double)> func)
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

double trapezoidal_integralSTLTR(const std::function<double(double, double)>& f, double lowerx, double upperx,
                                 double lowery, double uppery, int nx, int ny);
