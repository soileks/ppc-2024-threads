// Copyright 2024 Troitskiy Alexandr
#include "stl/troitsk_v_the_trapezoid_method/include/trapezoid_method_stl.hpp"

double trapezoidal_integralTR(const std::function<double(double, double)>& f, double lowerx, double upperx,
                              double lowery, double uppery, int nx, int ny) {
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

double trapezoidal_integralSTLTR(const std::function<double(double, double)>& f, double lowerx, double upperx,
                                 double lowery, double uppery, int nx, int ny) {
  double hx = (upperx - lowerx) / nx;
  double hy = (uppery - lowery) / ny;
  double sum = 0.5 * (f(lowerx, lowery) + f(upperx, uppery));

  std::vector<std::future<double>> futures(nx - 1);

  for (int i = 1; i < nx; ++i) {
    futures[i - 1] = std::async(
        std::launch::async,
        [&](int ii) {
          double x = lowerx + hx * ii;
          double inner_sum = 0.0;
          for (int j = 1; j < ny; ++j) {
            double y = lowery + hy * j;
            inner_sum += f(x, y);
          }
          return inner_sum;
        },
        i);
  }

  for (auto& future : futures) {
    sum += future.get();
  }

  return hx * hy * sum;
}

bool troitskTaskTBBSequential::pre_processing() {
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

bool troitskTaskTBBSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool troitskTaskTBBSequential::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool troitskTaskTBBSequential::run() {
  try {
    internal_order_test();
    res = trapezoidal_integralTR(f, lowerx, upperx, lowery, uppery, nx, ny);
  } catch (...) {
    return false;
  }
  return true;
}

bool troitskTaskSTL::pre_processing() {
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

bool troitskTaskSTL::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool troitskTaskSTL::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool troitskTaskSTL::run() {
  try {
    internal_order_test();
    res = trapezoidal_integralTR(f, lowerx, upperx, lowery, uppery, nx, ny);
  } catch (...) {
    return false;
  }
  return true;
}
