// Copyright 2024 Podyachikh Mikhail

#include "stl/podyachikh_m_hoare_sort_simple_merge/include/hoare_sort.hpp"

using namespace std::chrono_literals;
using namespace PodyachikhSTL;

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
  max_segment_length = _data.size() / 4;
  try {
    stl_hoare_sort(0, _data.size() - 1);
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

void HoareSort::seq_hoare_sort(int left, int right) {
  if (left >= right) return;
  int pivot = _data[(left + right) / 2];
  int i = left - 1;
  int j = right + 1;
  while (true) {
    do {
      i++;
    } while (_comp(_data[i], pivot));
    do {
      j--;
    } while (_comp(pivot, _data[j]));
    if (i >= j) {
      break;
    }
    std::swap(_data[i], _data[j]);
  }
  seq_hoare_sort(left, j);
  seq_hoare_sort(j + 1, right);
}

void HoareSort::stl_hoare_sort(int left, int right) {
  if (left >= right) return;
  if (right - left < max_segment_length) {
    return seq_hoare_sort(left, right);
  }
  int pivot = _data[(left + right) / 2];
  int i = left - 1;
  int j = right + 1;
  while (true) {
    do {
      i++;
    } while (_comp(_data[i], pivot));
    do {
      j--;
    } while (_comp(pivot, _data[j]));
    if (i >= j) {
      break;
    }
    std::swap(_data[i], _data[j]);
  }

  std::thread lp_thread(&HoareSort::stl_hoare_sort, this, left, j);
  std::thread rp_thread(&HoareSort::stl_hoare_sort, this, j + 1, right);

  lp_thread.join();
  rp_thread.join();
}
