// Copyright 2024 Chuvashov Andrey

#include "tbb/chuvashov_a_batcher_ints_sort/include/batcher_ints_sort.hpp"

#include <oneapi/tbb/parallel_for.h>

using namespace std::chrono_literals;

std::vector<int> ChuvashovTBB_BatcherEven(std::vector<int> arr1, std::vector<int> arr2) {
  std::vector<int> result(arr1.size() / 2 + arr2.size() / 2 + arr1.size() % 2 + arr2.size() % 2);
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;

  while ((j < arr1.size()) && (k < arr2.size())) {
    if (arr1[j] <= arr2[k]) {
      result[i] = arr1[j];
      j += 2;
    } else {
      result[i] = arr2[k];
      k += 2;
    }
    i++;
  }

  if (j >= arr1.size()) {
    for (int l = k; l < static_cast<int>(arr2.size()); l += 2) {
      result[i] = arr2[l];
      i++;
    }
  } else {
    for (int l = j; l < static_cast<int>(arr1.size()); l += 2) {
      result[i] = arr1[l];
      i++;
    }
  }

  return result;
}

std::vector<int> ChuvashovTBB_BatcherOdd(std::vector<int> arr1, std::vector<int> arr2) {
  std::vector<int> result(arr1.size() / 2 + arr2.size() / 2);
  size_t i = 0;
  size_t j = 1;
  size_t k = 1;

  while ((j < arr1.size()) && (k < arr2.size())) {
    if (arr1[j] <= arr2[k]) {
      result[i] = arr1[j];
      j += 2;
    } else {
      result[i] = arr2[k];
      k += 2;
    }
    i++;
  }

  if (j >= arr1.size()) {
    for (int l = k; l < static_cast<int>(arr2.size()); l += 2) {
      result[i] = arr2[l];
      i++;
    }
  } else {
    for (int l = j; l < static_cast<int>(arr1.size()); l += 2) {
      result[i] = arr1[l];
      i++;
    }
  }

  return result;
}

std::vector<int> ChuvashovTBB_merge(std::vector<int> arr1, std::vector<int> arr2, size_t threads_count = 4) {
  std::vector<int> result(arr1.size() + arr2.size());
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;

  while ((j < arr1.size()) && (k < arr2.size())) {
    result[i] = arr1[j];
    result[i + 1] = arr2[k];
    i += 2;
    j++;
    k++;
  }
  if ((k >= arr2.size()) && (j < arr1.size())) {
    for (int l = i; l < static_cast<int>(result.size()); l++) {
      result[l] = arr1[j];
      j++;
    }
  }
  tbb::parallel_for(
      tbb::blocked_range<int>(0, result.size() - 1, result.size() / threads_count),
      [&](const tbb::blocked_range<int> &r) {
        for (int l = r.begin(); l != r.end(); ++l) {
          if (result[l] > result[l + 1]) {
            std::swap(result[l], result[l + 1]);
          }
        }
      },
      tbb::auto_partitioner());

  return result;
}

std::vector<int> ChuvashovTBB_BatcherSort(const std::vector<int> &arr1, const std::vector<int> &arr2) {
  std::vector<int> even = ChuvashovTBB_BatcherEven(arr1, arr2);
  std::vector<int> odd = ChuvashovTBB_BatcherOdd(arr1, arr2);
  std::vector<int> result = ChuvashovTBB_merge(even, odd);
  return result;
}

bool Chuvashov_TBBBatcherSort::pre_processing() {
  internal_order_test();
  input = std::vector<int>(taskData->inputs_count[0]);
  auto *tmp_ptr_A = reinterpret_cast<int *>(taskData->inputs[0]);
  for (size_t i = 0; i < taskData->inputs_count[0]; i++) {
    input[i] = tmp_ptr_A[i];
  }
  arr1.resize(input.size() / 2);
  arr2.resize(input.size() / 2);

  for (size_t i = 0; i < (input.size() / 2); i++) {
    arr1[i] = input[i];
    arr2[i] = input[(input.size() / 2) + i];
  }

  std::sort(arr1.begin(), arr1.end());
  std::sort(arr2.begin(), arr2.end());

  return true;
}

bool Chuvashov_TBBBatcherSort::validation() {
  internal_order_test();
  return (std::is_sorted(arr1.begin(), arr1.end()) && std::is_sorted(arr2.begin(), arr2.end()));
}

bool Chuvashov_TBBBatcherSort::run() {
  internal_order_test();
  output = ChuvashovTBB_BatcherSort(arr1, arr2);
  return true;
}

bool Chuvashov_TBBBatcherSort::post_processing() {
  internal_order_test();
  std::copy(output.begin(), output.end(), reinterpret_cast<int *>(taskData->outputs[0]));
  return true;
}
