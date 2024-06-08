#include "omp/ryabkov_v_int_merge_batcher/include/int_merge_batcher.hpp"

namespace ryabkov_batcher {
void radix_sort(std::vector<int>& arr, int exp) {
  const std::size_t n = arr.size();
  std::vector<int> output(n);
  std::vector<int> count(10, 0);

#pragma omp parallel for
  for (std::size_t i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;

  for (int i = 1; i < 10; i++) count[i] += count[i - 1];

#pragma omp parallel for
  for (int i = static_cast<int>(n) - 1; i >= 0; i--) {
    output[count[(arr[i] / exp) % 10] - 1] = arr[i];
#pragma omp atomic
    count[(arr[i] / exp) % 10]--;
  }

#pragma omp parallel for
  for (std::size_t i = 0; i < n; i++) arr[i] = output[i];
}

void radix_sort(std::vector<int>& arr) {
  const int max_element = *std::max_element(arr.begin(), arr.end());

  for (int exp = 1; max_element / exp > 0; exp *= 10) radix_sort(arr, exp);
}

std::vector<int> batch_merge(const std::vector<int>& a1, const std::vector<int>& a2) {
  std::vector<int> merged(a1.size() + a2.size());
  std::size_t i = 0;
  std::size_t j = 0;

  for (std::size_t k = 0; k < merged.size(); ++k) {
    if (i < a1.size() && (j >= a2.size() || a1[i] < a2[j])) {
      merged[k] = a1[i++];
    } else {
      merged[k] = a2[j++];
    }
  }
  return merged;
}

std::vector<int> BatchSort(std::vector<int>& a1, std::vector<int>& a2) {
  std::vector<int> merged = batch_merge(a1, a2);

#pragma omp parallel for
  for (size_t bit = 0; bit < sizeof(int) * 8; bit++) {
    for (std::size_t i = 0; i < merged.size() / 2; i++) {
      if (((i % 2 == 0) && ((merged[2 * i] >> bit) & 1) != 0) ||
          ((i % 2 != 0) && ((merged[2 * i + 1] >> bit) & 1) != 0)) {
#pragma omp critical
        std::swap(merged[2 * i], merged[2 * i + 1]);
      }
    }
  }

  radix_sort(merged);

  std::size_t n = merged.size() / 2;
  a1.assign(merged.begin(), merged.begin() + n);
  a2.assign(merged.begin() + n, merged.end());

  return merged;
}
}  // namespace ryabkov_batcher

bool ryabkov_batcher::SeqBatcher::pre_processing() {
  internal_order_test();

  if (!taskData) return false;

  inv.resize(taskData->inputs_count[0]);
  int* tmp_ptr_A = reinterpret_cast<int*>(taskData->inputs[0]);

#pragma omp parallel for
  for (std::size_t i = 0; i < taskData->inputs_count[0]; ++i) {
#pragma omp critical
    {
      inv[i] = tmp_ptr_A[i];
      a1.push_back(inv[i]);
      a2.push_back(inv[taskData->inputs_count[0] / 2 + i]);
    }
  }

  return true;
}

bool ryabkov_batcher::SeqBatcher::validation() {
  internal_order_test();

  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool ryabkov_batcher::SeqBatcher::run() {
  internal_order_test();

  result = ryabkov_batcher::BatchSort(a1, a2);
  return true;
}

bool ryabkov_batcher::SeqBatcher::post_processing() {
  internal_order_test();

  int* tmp_ptr_A = reinterpret_cast<int*>(taskData->outputs[0]);
#pragma omp parallel for
  for (std::size_t i = 0; i < result.size(); ++i) {
#pragma omp critical
    { tmp_ptr_A[i] = result[i]; }
  }

  return true;
}