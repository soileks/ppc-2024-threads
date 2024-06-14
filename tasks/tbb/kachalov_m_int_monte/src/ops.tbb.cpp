// Copyright 2024 Kachalov Mikhail

#include <tbb/tbb.h>

#include <cmath>
#include <exception>
#include <iostream>
#include <thread>

#include "tbb/kachalov_m_int_monte/include/ops_tbb.hpp"

using namespace std::chrono_literals;

bool KachalovIntegralSequentialMonteCarlo::pre_processing() {
  internal_order_test();
  Int1[0] = reinterpret_cast<double*>(taskData->inputs[0])[0];
  Int1[1] = reinterpret_cast<double*>(taskData->inputs[0])[1];
  Int2[0] = reinterpret_cast<double*>(taskData->inputs[1])[0];
  Int2[1] = reinterpret_cast<double*>(taskData->inputs[1])[1];
  function = reinterpret_cast<integ_func>(taskData->inputs[2]);

  N = 100;
  res = 0.0;
  return true;
}

bool KachalovIntegralSequentialMonteCarlo::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 2 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool KachalovIntegralSequentialMonteCarlo::run() {
  internal_order_test();

  double h1 = (Int1[1] - Int1[0]) / N;
  double h2 = (Int2[1] - Int2[0]) / N;

  int i;
  int j;
  for (j = 0; j < N; ++j) {
    double y = Int2[0] + h2 * j;
    for (i = 0; i < N; ++i) {
      res += function(Int1[0] + h1 * i, y);
    }
  }
  res *= h1 * h2;

  return true;
}

bool KachalovIntegralSequentialMonteCarlo::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool KachalovIntegralTBBMonteCarlo::pre_processing() {
  internal_order_test();
  Int1[0] = reinterpret_cast<double*>(taskData->inputs[0])[0];
  Int1[1] = reinterpret_cast<double*>(taskData->inputs[0])[1];
  Int2[0] = reinterpret_cast<double*>(taskData->inputs[1])[0];
  Int2[1] = reinterpret_cast<double*>(taskData->inputs[1])[1];
  function = reinterpret_cast<integ_func>(taskData->inputs[2]);

  N = 1000;
  res = 0.0;
  return true;
}

bool KachalovIntegralTBBMonteCarlo::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 2 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool KachalovIntegralTBBMonteCarlo::run() {
  internal_order_test();
  N = 100;
  double h1 = (Int1[1] - Int1[0]) / N;
  double h2 = (Int2[1] - Int2[0]) / N;

  res = oneapi::tbb::parallel_reduce(
      oneapi::tbb::blocked_range<int>(0, N), 0.0,
      [&](const tbb::blocked_range<int>& r, double total) {
        for (int j = r.begin(); j < r.end(); ++j) {
          double y = Int2[0] + h2 * j;
          for (int i = 0; i < N; ++i) {
            total += function(Int1[0] + h1 * i, y);
          }
        }
        return total;
      },
      std::plus<>());
  res *= h1 * h2;

  return true;
}

bool KachalovIntegralTBBMonteCarlo::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}
