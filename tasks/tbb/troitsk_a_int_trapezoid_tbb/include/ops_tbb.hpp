// Copyright 2024 troitsk alexandr
#pragma once

#include <tbb/tbb.h>

#include <functional>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace troitsk_a_integral_trapezoid_tbb {

class TestTBBTaskSequentialtroitskIntegralTrapezoid : public ppc::core::Task {
 public:
  explicit TestTBBTaskSequentialtroitskIntegralTrapezoid(std::shared_ptr<ppc::core::TaskData> taskData_,
                                                         std::function<double(double, double)> fun_)
      : Task(std::move(taskData_)), fun(std::move(fun_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double a1{}, b1{}, a2{}, b2{};
  int n1{}, n2{};
  double res = 0.0;
  std::function<double(double, double)> fun;
};

class TestTBBTaskParalleltroitskIntegralTrapezoid : public ppc::core::Task {
 public:
  explicit TestTBBTaskParalleltroitskIntegralTrapezoid(std::shared_ptr<ppc::core::TaskData> taskData_,
                                                       std::function<double(double, double)> fun_)
      : Task(std::move(taskData_)), fun(std::move(fun_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double a1{}, b1{}, a2{}, b2{};
  int n1{}, n2{};
  double res = 0.0;
  std::function<double(double, double)> fun;
};

double trapezoidal_integral(double a1, double b1, double a2, double b2, int n1, int n2,
                            const std::function<double(double, double)> &fun);

double trapezoidal_integral_tbb(double a1, double b1, double a2, double b2, int n1, int n2,
                                const std::function<double(double, double)> &fun);
}  // namespace troitsk_a_integral_trapezoid_tbb