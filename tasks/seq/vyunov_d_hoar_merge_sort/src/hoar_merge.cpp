// Copyright 2024 Vyunov Danila
#include "./seq/vyunov_d_hoar_merge_sort/include/hoar_merge.h"

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
    std::this_thread::sleep_for(20ms);
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

void HoarSort(std::vector<int> *arr, int l, int r) {
  std::vector<int> &s = *arr;
  if (s.empty()) throw "Empty vector";
  int left = l;
  int right = r;
  int middle = s[(l + r) / 2];

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

  if (left < r) HoarSort(&s, left, r);
  if (l < right) HoarSort(&s, l, right);
}
