// Copyright 2024 Kachalov Mikhail
#include "stl/kachalov_m_int_monte/include/ops_stl.hpp"

#include <cmath>
#include <exception>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

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

bool KachalovIntegralSTLMonteCarlo::pre_processing() {
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

bool KachalovIntegralSTLMonteCarlo::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 2 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool KachalovIntegralSTLMonteCarlo::run() {
  internal_order_test();
  N = 100;
  double h1 = (Int1[1] - Int1[0]) / N;
  double h2 = (Int2[1] - Int2[0]) / N;

  auto compute = [this, h1, h2](int start, int end, double& local_res) {
    for (int j = start; j < end; ++j) {
      double y = Int2[0] + h2 * j;
      for (int i = 0; i < N; ++i) {
        local_res += function(Int1[0] + h1 * i, y);
      }
    }
  };

  int num_threads = std::thread::hardware_concurrency();
  int chunk_size = N / num_threads;

  std::vector<std::thread> threads;
  std::vector<double> results(num_threads, 0.0);

  for (int t = 0; t < num_threads; ++t) {
    int start = t * chunk_size;
    int end = (t == num_threads - 1) ? N : (t + 1) * chunk_size;
    threads.emplace_back(compute, start, end, std::ref(results[t]));
  }

  for (auto& th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  res = std::accumulate(results.begin(), results.end(), 0.0);
  res *= h1 * h2;

  return true;
}

bool KachalovIntegralSTLMonteCarlo::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}
