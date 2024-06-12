// Copyright 2024 Makhinya Danil

#include "tbb/makhinya_d_hoare_sort_batcher_merge/include/hoare_sort_tbb.hpp"

#include "tbb/tbb.h"

bool HoareSortTBB::pre_processing() {
  internal_order_test();
  // Init value for input and output
  _data = reinterpret_cast<vec_t*>(taskData->inputs[0]);
  if (taskData->inputs.size() == 2) {
    _comp = *(reinterpret_cast<compare_t*>(taskData->inputs[1]));
  } else {
    _comp = default_compare;
  }
  return true;
}

bool HoareSortTBB::validation() {
  internal_order_test();
  // Check count elements of output
  return (taskData->inputs.size() == 1 || taskData->inputs.size() == 2) && taskData->outputs.size() <= 1;
}

bool HoareSortTBB::run() {
  internal_order_test();
  sortable_type* begin = _data->data();
  sortable_type* end = begin + _data->size() - 1UL;
  parallel_hoare_sort(begin, end);
  return true;
}

bool HoareSortTBB::post_processing() {
  internal_order_test();
  return true;
}

bool HoareSortTBB::check_order() {
  for (uint32_t i = 1U; i < _data->size(); ++i) {
    if (_comp((*_data)[i], (*_data)[i - 1U])) {
      return true;
    }
  }

  return true;
}

void HoareSortTBB::sort_bitonic_seguence(sortable_type* first_ptr, sortable_type const* last_ptr) {
  const int32_t len = last_ptr - first_ptr;
  for (int32_t k = 1; k < len; k <<= 1) {
    const uint32_t chank = len / k;
    tbb::parallel_for(0, k, 1, [&](uint32_t i) {
      // k = 5
      sortable_type* l = first_ptr + i * chank;
      sortable_type* r = l + chank;
      if (_comp(*l, *r)) {
        std::swap(*r, *l);
      }
    });
  }
}

void HoareSortTBB::parallel_hoare_sort(sortable_type* first_ptr, sortable_type const* last_ptr) {
  const int32_t K = 16;
  const uint32_t chank = (last_ptr - first_ptr) / K;

  tbb::parallel_for(0, K, 2, [&](uint32_t i) {
    sortable_type* l1 = first_ptr + i * chank;
    sortable_type* r1 = l1 + chank;
    sortable_type* l2 = first_ptr + (i + 1) * chank;
    sortable_type* r2 = l2 + chank;
    seq_hoare_sort(l1, r1);
    seq_hoare_sort(l2, r2);
    sort_bitonic_seguence(l1, r2);
  });
}

void HoareSortTBB::seq_hoare_sort(sortable_type* first_ptr, sortable_type* last_ptr) {
  if (last_ptr <= first_ptr) {
    return;
  }

  sortable_type m = *(first_ptr + (last_ptr + 1UL - first_ptr) / 2UL);
  sortable_type* ll = first_ptr;
  sortable_type* rr = last_ptr;
  while (ll <= rr) {
    while (_comp(*ll, m)) ll++;
    while (_comp(m, *rr)) rr--;

    if (ll <= rr) {
      std::swap(*ll, *rr);
      ll++;
      rr--;
    }
  }

  if (first_ptr < rr) {
    HoareSortTBB::seq_hoare_sort(first_ptr, rr);
  }
  if (ll < last_ptr) {
    HoareSortTBB::seq_hoare_sort(ll, last_ptr);
  }
}
