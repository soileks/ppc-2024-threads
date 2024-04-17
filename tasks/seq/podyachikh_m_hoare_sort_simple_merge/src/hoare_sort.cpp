// Copyright 2024 Podyachikh Mikhail

#include "seq/podyachikh_m_hoare_sort_simple_merge/include/hoare_sort.hpp"

#include <cassert>

using namespace std::chrono_literals;
using namespace Podyachikh;

bool HoareSort::pre_processing() {
  internal_order_test();
  try {
    // Init value for input and output
    _data = reinterpret_cast<vec_t *>(taskData->inputs[0])[0];
    if (taskData->inputs.size() == 2) {
      _comp = reinterpret_cast<compare_t *>(taskData->inputs[1])[0];
    } else {
      _comp = std::less<>();
    }
  } catch (...) {
    std::cout << "Failed pre_processing\n" << std::endl;
    return false;
  }
  return true;
}

bool HoareSort::validation() {
  internal_order_test();
  // Check count elements of output
  if (taskData->inputs.empty()) return false;
  if (taskData->inputs.size() > 2) return false;
  return taskData->outputs.size() == 1;
}

bool HoareSort::run() {
  internal_order_test();
  try {
    seq_hoare_sort(0, _data.size() - 1);
  } catch (...) {
    std::cout << "Run failure\n" << std::endl;
    return false;
  }
  return true;
}

bool HoareSort::post_processing() {
  internal_order_test();
  auto &res = reinterpret_cast<vec_t *>(taskData->outputs[0])[0];
  for (size_t i = 0; i < _data.size(); i++) {
    res[i] = _data[i];
  }
  return true;
}

int HoareSort::partition(int left, int right) {
  int pivot = _data[(left + right) / 2];
  left--;
  right++;
  while (true) {
    while (_comp(_data[++left], pivot))
      ;
    while (_comp(pivot, _data[--right]))
      ;
    if (left >= right) {
      return right;
    }
    std::swap(_data[left], _data[right]);
  }
}

void HoareSort::seq_hoare_sort(int left, int right) {
  if (left >= right) return;
  int mid = partition(left, right);
  seq_hoare_sort(left, mid);
  seq_hoare_sort(mid + 1, right);
}
