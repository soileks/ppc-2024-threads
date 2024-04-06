// Copyright 2024 Ryabkov Vladislav

#include "seq/ryabkov_v_int_merge_batcher/include/int_merge_batcher.hpp"
using namespace std::chrono_literals;

using len_t = std::size_t;

void FMiss(std::vector<int>& result, const std::vector<int>& a1, const std::vector<int>& a2, len_t& i, len_t j,
           len_t k) {
  while (j < a1.size()) {
    result[i] = a1[j];
    ++i;
    j += 2;
  }
  while (k < a2.size()) {
    result[i] = a2[k];
    ++i;
    k += 2;
  }
}

void FRes(std::vector<int>& result, const std::vector<int>& a1, const std::vector<int>& a2, len_t& i, len_t& j,
          len_t& k) {
  while (j < a1.size() && k < a2.size()) {
    if (a1[j] <= a2[k]) {
      result[i] = a1[j];
      j += 2;
    } else {
      result[i] = a2[k];
      k += 2;
    }
    ++i;
  }
  FMiss(result, a1, a2, i, j, k);
}

std::vector<int> comb(const std::vector<int>& vec1, const std::vector<int>& vec2) {
  std::vector<int> result(vec1.size() + vec2.size());
  len_t i = 0;
  len_t j = 0;
  len_t k = 0;

  for (; j < vec1.size() && k < vec2.size(); i += 2, ++j, ++k) {
    result[i] = vec1[j];
    result[i + 1] = vec2[k];
  }

  for (i = 0; i < result.size() - 1; ++i) {
    if (result[i] > result[i + 1]) {
      std::swap(result[i], result[i + 1]);
    }
  }

  return result;
}

std::vector<int> merge_and_adjust(const std::vector<int>& a1, const std::vector<int>& a2) {
  std::vector<int> result(a1.size() / 2 + a2.size() / 2 + a1.size() % 2 + a2.size() % 2);
  len_t i = 0;
  len_t j = 0;
  len_t k = 0;

  FRes(result, a1, a2, i, j, k);

  return result;
}

std::vector<int> combine_and_fill(const std::vector<int>& a1, const std::vector<int>& a2) {
  std::vector<int> result(a1.size() / 2 + a2.size() / 2);
  len_t i = 0;
  len_t j = 1;
  len_t k = 1;

  FRes(result, a1, a2, i, j, k);

  return result;
}

std::vector<int> bat_sort(const std::vector<int>& a1, const std::vector<int>& a2) {
  std::vector<int> even_vec = merge_and_adjust(a1, a2);
  std::vector<int> odd_vec = combine_and_fill(a1, a2);
  std::vector<int> result = comb(even_vec, odd_vec);
  return result;
}

bool SeqBatcher::pre_processing() {
  internal_order_test();

  if (!taskData) return false;

  inv.resize(taskData->inputs_count[0]);
  int* tmp_ptr_A = reinterpret_cast<int*>(taskData->inputs[0]);
  std::copy(tmp_ptr_A, tmp_ptr_A + taskData->inputs_count[0], inv.begin());

  a1.resize(inv.size() / 2);
  a2.resize(inv.size() / 2);

  for (len_t i = 0; i < inv.size() / 2; ++i) {
    a1[i] = inv[i];
    a2[i] = inv[inv.size() / 2 + i];
  }

  std::sort(a1.begin(), a1.end());
  std::sort(a2.begin(), a2.end());

  return true;
}

bool SeqBatcher::validation() {
  internal_order_test();

  return std::is_sorted(a1.begin(), a1.end()) && std::is_sorted(a2.begin(), a2.end());
}

bool SeqBatcher::run() {
  internal_order_test();

  result = bat_sort(a1, a2);
  std::this_thread::sleep_for(20ms);
  return true;
}

bool SeqBatcher::post_processing() {
  internal_order_test();

  std::copy(result.begin(), result.end(), reinterpret_cast<int*>(taskData->outputs[0]));
  return true;
}
