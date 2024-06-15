// Copyright 2024 Pozdnyakov Vasya
#pragma once

#include <cmath>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace pozdnyakov_stl {

using Func = double (*)(double, double);

double pozdnyakov_flin(double x, double y);
double pozdnyakov_fxy(double x, double y);
double pozdnyakov_fysinx(double x, double y);
double pozdnyakov_fxexpy(double x, double y);

class PozdnyakovTaskSTL : public ppc::core::Task {
 public:
  explicit PozdnyakovTaskSTL(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  Func f{};
  double x1{}, x2{}, y1{}, y2{};
  int n;
  double res;
};

class PozdnyakovTaskSequential : public ppc::core::Task {
 public:
  explicit PozdnyakovTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  Func f{};
  double x1{}, x2{}, y1{}, y2{};
  int n;
  double res;
};
}  // namespace pozdnyakov_stl
