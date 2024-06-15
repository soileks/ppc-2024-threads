// Copyright 2024 Vyunov Danila
#include "tbb/vyunov_d_hoar_merge_sort_tbb/include/hoar_merge.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

bool HoareSortSimpleSeq::pre_processing() {
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

bool HoareSortSimpleSeq::validation() {
  try {
    internal_order_test();
  } catch (...) {
    return false;
  }
  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool HoareSortSimpleSeq::run() {
  try {
    internal_order_test();
    HoarSort(&array, 0, array.size() - 1);
  } catch (...) {
    return false;
  }
  return true;
}

bool HoareSortSimpleSeq::post_processing() {
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

std::vector<int> Getvec(int n) {
  std::random_device randomDevice;
  std::mt19937 engine(randomDevice());
  std::uniform_int_distribution<> dist(0, 100);
  std::vector<int> vec(n);
  for (int i = 0; i < n; i++) {
    vec[i] = dist(engine);
  }
  return vec;
}

void HoarSort(std::vector<int> *arr, int first, int last) {
  std::vector<int> &s = *arr;
  if (s.empty()) throw "Empty vector";
  int left = first;
  int right = last;
  int middle = s[(first + last) / 2];

  do {
    while (s[left] < middle) left++;
    while (s[right] > middle) right--;

    if (left <= right) {
      if (s[left] > s[right]) {
        std::swap(s[left], s[right]);
      }
      left++;
      right--;
    }
  } while (left <= right);

  if (left < last) HoarSort(&s, left, last);
  if (first < right) HoarSort(&s, first, right);
}

bool HoareSortTBBV::pre_processing() {
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

bool HoareSortTBBV::validation() {
  try {
    internal_order_test();
  } catch (...) {
    return false;
  }
  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool HoareSortTBBV::run() {
  try {
    internal_order_test();
    HoareSortParallel(array, 0, array.size() - 1);
  } catch (...) {
    return false;
  }
  return true;
}

bool HoareSortTBBV::post_processing() {
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

void HoareSortTBBV::HoareSortParallel(std::vector<int> &arr, size_t l, size_t r) {
  if (arr.size() <= 1) return;
  int n = r - l + 1;

  for (int p = 1; p < n; p += p)
    for (int k = p; k > 0; k /= 2)
      for (int j = k % p; j + k < n; j += (k + k))
        oneapi::tbb::parallel_for(oneapi::tbb::blocked_range<int>(0, n - j - k),
                                  [&](oneapi::tbb::blocked_range<int> &R) {
                                    for (int i = R.begin(); i < R.end(); ++i) {
                                      if ((j + i) / (p + p) == (j + i + k) / (p + p))
                                        if (arr[l + j + i] > arr[l + j + i + k])
                                          std::swap(arr[l + j + i], arr[l + j + i + k]);
                                    }
                                  });
}
