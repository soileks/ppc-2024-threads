// Copyright 2024 Kanakov Roman
#include "seq/kanakov_r_hoare_sort/include/hoare_sort.hpp"

#include <algorithm>
#include <random>
#include <thread>

using namespace std::chrono_literals;

bool HoareSortSequential::pre_processing() {
  try {
    internal_order_test();
    array.clear();
    for (size_t i = 0; i < taskData->inputs_count[0]; ++i) {
      int *currentElementPtr = reinterpret_cast<int *>(taskData->inputs[0] + i * sizeof(int));
      array.push_back(*currentElementPtr);
    }
  } catch (...) {
    return false;
  }
  return true;
}

bool HoareSortSequential::validation() {
  try {
    internal_order_test();
  } catch (...) {
    return false;
  }
  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool HoareSortSequential::run() {
  try {
    internal_order_test();
    hoareSortSequential(array, 0, array.size() - 1);
    std::this_thread::sleep_for(20ms);
  } catch (...) {
    return false;
  }
  return true;
}

bool HoareSortSequential::post_processing() {
  try {
    internal_order_test();
    for (size_t i = 0; i < array.size(); ++i) {
      int *currentElementPtr = reinterpret_cast<int *>(taskData->outputs[0] + i * sizeof(int));
      *currentElementPtr = array[i];
    }
  } catch (...) {
    return false;
  }
  return true;
}

void HoareSortSequential::hoareSortSequential(std::vector<int> &arr, size_t l, size_t r) {
  if (arr.size() <= 1) return;
  int n = r - l + 1;
  for (int m = 1; m < n; m += m)
    for (int k = m; k > 0; k /= 2)
      for (int j = k % m; j + k < n; j += (k + k))
        for (int i = 0; i < n - j - k; ++i)
          if ((j + i) / (m + m) == (j + i + k) / (m + m)) CompExch(arr[l + j + i], arr[l + j + i + k]);
}

void HoareSortSequential::CompExch(int &a, int &b) {
  if (a > b) std::swap(a, b);
}

std::vector<int> generateRandomVector(int size, int min, int max) {
  std::vector<int> res(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(min, max);
  for (int i = 0; i < size; ++i) res[i] = distrib(gen);
  return res;
}
