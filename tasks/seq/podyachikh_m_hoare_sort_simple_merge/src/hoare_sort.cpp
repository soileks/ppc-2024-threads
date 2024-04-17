// Copyright 2024 Podyachikh Mikhail

#include "seq/podyachikh_m_hoare_sort_simple_merge/include/hoare_sort.hpp"

#include <cassert>

using namespace std::chrono_literals;
using namespace Podyachikh;

bool HoareSort::pre_processing() {
  internal_order_test();
  try {
    // Init value for input and output
    _data = reinterpret_cast<vec_t*>(taskData->inputs[0]);
    if (taskData->inputs.size() == 2) {
      _comp = reinterpret_cast<compare_t*>(taskData->inputs[1])[0];
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
  if (taskData->outputs.size() > 1) return false;
  if (taskData->outputs.empty()) {
    return true;
  }
  return taskData->outputs[0] == nullptr;
}

bool HoareSort::run() {
  internal_order_test();
  try {
    obj_ty* begin = _data->data();
    obj_ty* end = _data->data() + _data->size() - 1;
    seq_hoare_sort(begin, end);
  } catch (...) {
    std::cout << "Run failure\n" << std::endl;
    return false;
  }
  return true;
}

bool HoareSort::post_processing() {
  internal_order_test();
  return true;
}

HoareSort::obj_ty* HoareSort::get_random_pivot(obj_ty* left, obj_ty* right) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dis(0, right - left);
  return left + dis(gen);
}

std::pair<HoareSort::obj_ty*, HoareSort::obj_ty*> HoareSort::partition(obj_ty* left, obj_ty* right) {
  obj_ty pivot = *get_random_pivot(left, right);
  obj_ty* i = left;
  obj_ty* j = right;
  while (true) {
    while (_comp(*i, pivot)) i++;
    while (_comp(pivot, *j)) j--;

    if (i > j) {
      return {j, i};
    }
    std::swap(*(i++), *(j--));
  }
}

void HoareSort::seq_hoare_sort(obj_ty* left, obj_ty* right) {
  if (left >= right) return;
  auto [i, j] = partition(left, right);
  seq_hoare_sort(left, i);
  seq_hoare_sort(j, right);
}
