// Copyright 2024 Zhatkin Vyacheslav
#include "omp/zhatkin_v_the_trapezoid_method/include/trapezoid_method_omp.hpp"

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

double trapezoidal_integralOMP(const std::function<double(double, double)>& f, double lowerx, double upperx,
                               double lowery, double uppery, int nx, int ny) {
  double hx = (upperx - lowerx) / nx;
  double hy = (uppery - lowery) / ny;
  double sum = 0.5 * (f(lowerx, lowery) + f(upperx, uppery));

#pragma omp parallel for reduction(+ : sum) collapse(2)
  for (int i = 1; i < nx; ++i) {
    double x = lowerx + hx * i;
    for (int j = 1; j < ny; ++j) {
      double y = lowery + hy * j;
      sum += f(x, y);
    }
  }

  return hx * hy * sum;
}

bool ZhatkinTaskSequential::pre_processing() {
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

bool ZhatkinTaskSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool ZhatkinTaskSequential::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool ZhatkinTaskSequential::run() {
  try {
    internal_order_test();
    res = trapezoidal_integral(f, lowerx, upperx, lowery, uppery, nx, ny);
  } catch (...) {
    return false;
  }
  return true;
}

bool ZhatkinTaskOMP::pre_processing() {
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

bool ZhatkinTaskOMP::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool ZhatkinTaskOMP::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool ZhatkinTaskOMP::run() {
  try {
    internal_order_test();
    res = trapezoidal_integral(f, lowerx, upperx, lowery, uppery, nx, ny);
  } catch (...) {
    return false;
  }
  return true;
}
