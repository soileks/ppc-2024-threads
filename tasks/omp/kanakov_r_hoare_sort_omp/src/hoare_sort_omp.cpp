// Copyright 2024 Kanakov Roman
#include "omp/kanakov_r_hoare_sort_omp/include/hoare_sort_omp.hpp"
#include <omp.h>
#include <algorithm>
#include <random>
#include <thread>

using namespace std::chrono_literals;

bool kanakov_omp::HoareSortSequential::pre_processing() {
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

bool kanakov_omp::HoareSortSequential::validation() {
  try {
    internal_order_test();
  } catch (...) {
    return false;
  }
  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool kanakov_omp::HoareSortSequential::run() {
  try {
    internal_order_test();
    HoareSortSeq(array, 0, array.size() - 1);
  } catch (...) {
    return false;
  }
  return true;
}

bool kanakov_omp::HoareSortSequential::post_processing() {
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

void kanakov_omp::HoareSortSequential::HoareSortSeq(std::vector<int> &arr, size_t l,
                                                                               size_t r) {
  if (arr.size() <= 1) return;
  int n = r - l + 1;
  for (int p = 1; p < n; p += p)
    for (int k = p; k > 0; k /= 2)
      for (int j = k % p; j + k < n; j += (k + k))
        for (int i = 0; i < n - j - k; ++i)
          if ((j + i) / (p + p) == (j + i + k) / (p + p)) if (arr[l + j + i] > arr[l + j + i + k]) std::swap(arr[l + j + i], arr[l + j + i + k]);
}

bool kanakov_omp::HoareSortOMP::pre_processing() {
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

bool kanakov_omp::HoareSortOMP::validation() {
  try {
    internal_order_test();
  } catch (...) {
    return false;
  }
  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool kanakov_omp::HoareSortOMP::run() {
  try {
    internal_order_test();
    HoareSortParallel(array, 0, array.size() - 1);
  } catch (...) {
    return false;
  }
  return true;
}

bool kanakov_omp::HoareSortOMP::post_processing() {
  try {
    internal_order_test();
    if (array.size() != taskData->outputs_count[0]) {
      throw;
    }
    for (size_t i = 0; i < array.size(); ++i) {
      int *currentElementPtr = reinterpret_cast<int *>(taskData->outputs[0] + i * sizeof(int));
      *currentElementPtr = array[i];
    }
  } catch (...) {
    return false;
  }
  return true;
}

void kanakov_omp::HoareSortOMP::HoareSortParallel(std::vector<int> &arr, size_t l, size_t r) {
  if (arr.size() <= 1) return;
  int n = r - l + 1;

  for (int p = 1; p < n; p += p)
    for (int k = p; k > 0; k /= 2)
      for (int j = k % p; j + k < n; j += (k + k))
#pragma omp parallel for
        for (int i = 0; i < n - j - k; ++i)
          if ((j + i) / (p + p) == (j + i + k) / (p + p))
            if (arr[l + j + i] > arr[l + j + i + k]) std::swap(arr[l + j + i], arr[l + j + i + k]);
}