// Copyright 2024 Bozin Dmitry
#pragma once

#include <tbb/tbb.h>

#include <functional>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace Bozin_d_tbb {

class BozinTaskSequential : public ppc::core::Task {
 public:
  explicit BozinTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_,
                               std::function<double(double, double)> func_)
      : Task(std::move(taskData_)), func(std::move(func_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double res = 0.0;
  double ax{}, bx{}, ay{}, by{};
  int ny{}, nx{};
  std::function<double(double, double)> func;
};

class BozinTaskParralel : public ppc::core::Task {
 public:
  explicit BozinTaskParralel(std::shared_ptr<ppc::core::TaskData> taskData_,
                             std::function<double(double, double)> func_)
      : Task(std::move(taskData_)), func(std::move(func_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double res = 0.0;
  double ax{}, bx{}, ay{}, by{};
  int ny{}, nx{};
  std::function<double(double, double)> func;
};
double multiDimensionalIntegral(const std::function<double(double, double)>& func, double ax, double bx, double ay,
                                double by, int nx, int ny);
double multiDimensionalIntegralTBB(const std::function<double(double, double)>& func, double ax, double bx, double ay,
                                   double by, int nx, int ny);
}  // namespace Bozin_d_tbb