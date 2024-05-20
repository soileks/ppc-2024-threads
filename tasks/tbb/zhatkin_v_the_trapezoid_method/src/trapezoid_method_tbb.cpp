// Copyright 2024 Zhatkin Vyacheslav
#include "tbb/zhatkin_v_the_trapezoid_method/include/trapezoid_method_tbb.hpp"

double trapezoidal_integral(const std::function<double(double, double)>& f, double lowerx, double upperx, double lowery,
                            double uppery, int nx, int ny) {
  double hx = (upperx - lowerx) / nx;
  double hy = (uppery - lowery) / ny;
  double sum = 0.5 * (f(lowerx, lowery) + f(upperx, uppery));

  for (int i = 1; i < nx; ++i) {
    double x = lowerx + hx * i;
    for (int j = 1; j < ny; ++j) {
      double y = lowery + hy * j;
      sum += f(x, y);
    }
  }

  return hx * hy * sum;
}

double trapezoidal_integralTBB(const std::function<double(double, double)>& f, double lowerx, double upperx,
                               double lowery, double uppery, int nx, int ny) {
  double hx = (upperx - lowerx) / nx;
  double hy = (uppery - lowery) / ny;
  double sum = 0.5 * (f(lowerx, lowery) + f(upperx, uppery));

  oneapi::tbb::combinable<double> localSums;

  oneapi::tbb::parallel_for(oneapi::tbb::blocked_range<int>(1, nx),
                            [&](const oneapi::tbb::blocked_range<int>& range) {
    auto& localSum = localSums.local();
    for (int i = range.begin(); i < range.end(); ++i) {
      double x = lowerx + hx * i;
      for (int j = 1; j < ny; ++j) {
        double y = lowery + hy * j;
        localSum += f(x, y);
      }
    }
  });

  sum += localSums.combine(std::plus<double>());

  return hx * hy * sum;
}

bool ZhatkinTaskTBBSequential::pre_processing() {
  internal_order_test();

  try {
    lowerx = reinterpret_cast<double*>(taskData->inputs[0])[0];
    upperx = reinterpret_cast<double*>(taskData->inputs[0])[1];
    lowery = reinterpret_cast<double*>(taskData->inputs[0])[2];
    uppery = reinterpret_cast<double*>(taskData->inputs[0])[3];
    nx = reinterpret_cast<int*>(taskData->inputs[1])[0];
    ny = reinterpret_cast<int*>(taskData->inputs[1])[1];
  } catch (...) {
    return false;
  }

  return true;
}

bool ZhatkinTaskTBBSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool ZhatkinTaskTBBSequential::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool ZhatkinTaskTBBSequential::run() {
  try {
    internal_order_test();
    res = trapezoidal_integral(f, lowerx, upperx, lowery, uppery, nx, ny);
  } catch (...) {
    return false;
  }
  return true;
}

bool ZhatkinTaskTBB::pre_processing() {
  internal_order_test();

  try {
    lowerx = reinterpret_cast<double*>(taskData->inputs[0])[0];
    upperx = reinterpret_cast<double*>(taskData->inputs[0])[1];
    lowery = reinterpret_cast<double*>(taskData->inputs[0])[2];
    uppery = reinterpret_cast<double*>(taskData->inputs[0])[3];
    nx = reinterpret_cast<int*>(taskData->inputs[1])[0];
    ny = reinterpret_cast<int*>(taskData->inputs[1])[1];
  } catch (...) {
    return false;
  }

  return true;
}

bool ZhatkinTaskTBB::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool ZhatkinTaskTBB::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool ZhatkinTaskTBB::run() {
  try {
    internal_order_test();
    res = trapezoidal_integral(f, lowerx, upperx, lowery, uppery, nx, ny);
  } catch (...) {
    return false;
  }
  return true;
}
