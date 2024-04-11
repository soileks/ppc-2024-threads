// Copyright 2024 Kiselev Igor
#include "seq/kiselev_i_shell_simple/include/shell_simple.hpp"

#include <thread>

using namespace std::chrono_literals;

bool KiselevTaskSequential::pre_processing() {
  try {
    internal_order_test();
    input_ = *reinterpret_cast<std::vector<int>*>(taskData->inputs[0]);
    size_t size = taskData->inputs_count[0];
    arr.resize(size);
    for (unsigned long i = 0; i < arr.size(); i++) {
      arr[i] = input_[i];
    }
    return true;
  } catch (char* ex) {
    (void)ex;
    return false;
  }
}

bool KiselevTaskSequential::validation() {
   try {
     internal_order_test();
     return taskData->inputs_count[0] != 0 && taskData->inputs_count[0] == taskData->outputs_count[0];
   } catch (char* ex) {
     (void)ex;
     return false;
   }
}

bool KiselevTaskSequential::run() {
  try {
    internal_order_test();
    int n = arr.size();
    int incr = n / 2;
    while (incr > 0) {
      for (int i = incr + 1; i < n; i++) {
        int j = i - incr;
        while (j > 0)
          if (arr[j] > arr[j + incr]) {
            arr[j] += arr[j + incr];
            arr[j + incr] = arr[j] - arr[j + incr];
            arr[j] = arr[j] - arr[j + incr];
            j = j - incr;
          } else
            j = 0;
      }
      incr = incr / 2;
    }
    return true;
  } catch (char* ex) {
    (void)ex;
    return false;
  }
}

bool KiselevTaskSequential::post_processing() {
  try {
    res = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
    internal_order_test();
    size_t n = arr.size();
    for (size_t i = 0; i < n; i++) {
      res[i] = arr[i];
      reinterpret_cast<int*>(taskData->outputs[i])[0] = res[i];
    }
    return true;
  } catch (char* ex) {
    (void)ex;
    return false;
  }
}
