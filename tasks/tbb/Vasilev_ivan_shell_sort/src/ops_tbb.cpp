// Copyright 2024 Vasilev Ivan
// #include <thread>

#include "tbb/Vasilev_ivan_shell_sort/include/ops_tbb.hpp"

#include <tbb/tbb.h>

#include <algorithm>
// using namespace std::chrono_literals;

bool VasilevTaskTbb::pre_processing() {
  internal_order_test();
  // Init value for input and output
  input_ = *reinterpret_cast<std::vector<int>*>(taskData->inputs[0]);
  res = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
  return true;
}

bool VasilevTaskTbb::validation() {
  internal_order_test();
  // Check count elements of output
  // return taskData->inputs_count[0] == 1 && taskData->outputs_count[0] == 1;
  return true;
}

bool VasilevTaskTbb::run() {
  internal_order_test();
  shell_sort_batcher(input_, input_.size());
  return true;
}

bool VasilevTaskTbb::post_processing() {
  internal_order_test();
  // reinterpret_cast<int*>(taskData->outputs[0])[0] = res;
  reinterpret_cast<std::vector<int>*>(taskData->outputs[0])->operator=(input_);

  return true;
}

void VasilevTaskTbb::shell_sort_batcher(std::vector<int>& arr, int n) {
  for (int gap = n / 2; gap > 0; gap /= 2) {
    bool sorted = false;
    while (!sorted) {
      sorted = true;

      tbb::parallel_for(tbb::blocked_range<int>(0, n - gap, 1), [&](const tbb::blocked_range<int>& r) {
        for (int i = r.begin(); i != r.end(); ++i) {
          if (i % 2 == 0 && arr[i] > arr[i + gap]) {
            std::swap(arr[i], arr[i + gap]);
            sorted = false;
          }
        }
      });

      tbb::parallel_for(tbb::blocked_range<int>(0, n - gap, 1), [&](const tbb::blocked_range<int>& r) {
        for (int i = r.begin(); i != r.end(); ++i) {
          if (i % 2 == 1 && arr[i] > arr[i + gap]) {
            std::swap(arr[i], arr[i + gap]);
            sorted = false;
          }
        }
      });
    }
  }
}

bool VasilevTaskTbb::R_sorted(std::vector<int> input) { return std::is_sorted(input.begin(), input.end()); }

std::vector<int> VasilevTaskTbb::generate_random_vector(int size, int min, int max) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine{rnd_device()};  // Generates random integers
  std::uniform_int_distribution<int> dist{min, max};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

  std::vector<int> vec(size);
  generate(begin(vec), end(vec), gen);

  return vec;
}
