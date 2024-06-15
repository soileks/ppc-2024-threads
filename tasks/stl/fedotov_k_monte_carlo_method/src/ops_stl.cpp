// Copyright 2024 Fedotov Kirill
#include "stl/fedotov_k_monte_carlo_method/include/ops_stl.hpp"

#include <cmath>
#include <thread>

using namespace std::chrono_literals;

double func_x_plus_y(double num1, double num2) { return num1 + num2; }

bool fedotov_stl::FedotovTaskSeq::pre_processing() {
  internal_order_test();
  Coordinates_For_Integration2[0] = reinterpret_cast<double*>(taskData->inputs[1])[0];
  Coordinates_For_Integration2[1] = reinterpret_cast<double*>(taskData->inputs[1])[1];
  Coordinates_For_Integration1[0] = reinterpret_cast<double*>(taskData->inputs[0])[0];
  Coordinates_For_Integration1[1] = reinterpret_cast<double*>(taskData->inputs[0])[1];
  function = reinterpret_cast<double (*)(double, double)>(taskData->inputs[2]);

  part_of_integrate = 1000;
  integration_result = 0.0;
  return true;
}

bool fedotov_stl::FedotovTaskSeq::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 2 && taskData->inputs_count[1] == 2 && taskData->outputs_count[0] == 1;
}

bool fedotov_stl::FedotovTaskSeq::run() {
  internal_order_test();

  double vysota1 = (Coordinates_For_Integration1[1] - Coordinates_For_Integration1[0]) / part_of_integrate;
  double vysota2 = (Coordinates_For_Integration2[1] - Coordinates_For_Integration2[0]) / part_of_integrate;

  int num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(num_threads);

  std::vector<double> partial_results(num_threads, 0.0);

  for (int t = 0; t < num_threads; ++t) {
    threads[t] = std::thread([&, t]() {
      double local_integration_result = 0.0;
      for (int k = t; k < part_of_integrate; k += num_threads) {
        double lmb = Coordinates_For_Integration2[0] + vysota2 * k;
        for (int r = 0; r < part_of_integrate; ++r) {
          local_integration_result += function(Coordinates_For_Integration1[0] + vysota1 * r, lmb);
        }
      }
      partial_results[t] = local_integration_result * vysota1 * vysota2;
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  for (double partial_result : partial_results) {
    integration_result += partial_result;
  }

  return true;
}

bool fedotov_stl::FedotovTaskSeq::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = integration_result;
  return true;
}
