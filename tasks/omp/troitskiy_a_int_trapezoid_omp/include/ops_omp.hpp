// Copyright 2024 Troitskiy Alexandr
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace troitskiy_a_omp_integral_trapezoid {

class TestTaskSequentialtroitskiyIntegralTrapezoid : public ppc::core::Task {
 public:
  explicit TestTaskSequentialtroitskiyIntegralTrapezoid(std::shared_ptr<ppc::core::TaskData> taskData_,
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

class TestOMPTaskParalleltroitskiyIntegralTrapezoid : public ppc::core::Task {
 public:
  explicit TestOMPTaskParalleltroitskiyIntegralTrapezoid(std::shared_ptr<ppc::core::TaskData> taskData_,
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

double trapezoidal_integral_omp(double a1, double b1, double a2, double b2, int n1, int n2,
                                const std::function<double(double, double)> &fun);
}  // namespace troitskiy_a_omp_integral_trapezoid