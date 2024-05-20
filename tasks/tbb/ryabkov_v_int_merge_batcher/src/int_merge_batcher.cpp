// Copyright 2024 Ryabkov Vladislav
#include "tbb/ryabkov_v_int_merge_batcher/include/int_merge_batcher.hpp"

namespace ryabkov_batcher {

void parallel_radix_sort(std::vector<int>& arr, int exp) {
  const std::size_t n = arr.size();
  std::vector<int> output(n);
  std::vector<int> count(10, 0);

  tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](const tbb::blocked_range<size_t>& r) {
    for (std::size_t i = r.begin(); i != r.end(); ++i) {
      count[(arr[i] / exp) % 10]++;
    }
  });

  for (int i = 1; i < 10; i++) {
    count[i] += count[i - 1];
  }

  for (int i = n - 1; i >= 0; --i) {
    output[count[(arr[i] / exp) % 10] - 1] = arr[i];
    count[(arr[i] / exp) % 10]--;
  }

  tbb::parallel_for(tbb::blocked_range<size_t>(0, n), [&](const tbb::blocked_range<size_t>& r) {
    for (std::size_t i = r.begin(); i != r.end(); ++i) {
      arr[i] = output[i];
    }
  });
}

void parallel_radix_sort(std::vector<int>& arr) {
  const int max_element = *std::max_element(arr.begin(), arr.end());

  for (int exp = 1; max_element / exp > 0; exp *= 10) {
    parallel_radix_sort(arr, exp);
  }
}

std::vector<int> parallel_batch_merge(const std::vector<int>& a1, const std::vector<int>& a2) {
  std::vector<int> merged(a1.size() + a2.size());
  std::size_t i = 0;
  std::size_t j = 0;
  std::size_t k = 0;

  while (i < a1.size() && j < a2.size()) {
    if (a1[i] < a2[j]) {
      merged[k++] = a1[i++];
    } else {
      merged[k++] = a2[j++];
    }
  }

  while (i < a1.size()) {
    merged[k++] = a1[i++];
  }

  while (j < a2.size()) {
    merged[k++] = a2[j++];
  }

  return merged;
}

std::vector<int> ParallelBatchSort(std::vector<int>& a1, std::vector<int>& a2) {
  std::vector<int> merged = parallel_batch_merge(a1, a2);

  parallel_radix_sort(merged);

  std::size_t n = merged.size() / 2;
  a1.assign(merged.begin(), merged.begin() + n);
  a2.assign(merged.begin() + n, merged.end());

  return merged;
}

bool SeqBatcher::pre_processing() {
  internal_order_test();

  if (!taskData) return false;

  inv.resize(taskData->inputs_count[0]);
  int* tmp_ptr_A = reinterpret_cast<int*>(taskData->inputs[0]);
  std::copy(tmp_ptr_A, tmp_ptr_A + taskData->inputs_count[0], inv.begin());

  a1.resize(inv.size() / 2);
  a2.resize(inv.size() / 2);

  for (std::size_t i = 0; i < inv.size() / 2; ++i) {
    a1[i] = inv[i];
    a2[i] = inv[inv.size() / 2 + i];
  }

  return true;
}

bool SeqBatcher::validation() {
  internal_order_test();

  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool SeqBatcher::run() {
  internal_order_test();

  result = ParallelBatchSort(a1, a2);
  return true;
}

bool SeqBatcher::post_processing() {
  internal_order_test();

  std::copy(result.begin(), result.end(), reinterpret_cast<int*>(taskData->outputs[0]));
  return true;
}

}  // namespace ryabkov_batcher
