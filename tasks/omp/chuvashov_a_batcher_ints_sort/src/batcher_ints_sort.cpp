// Copyright 2024 Chuvashov Andrey

#include "omp/chuvashov_a_batcher_ints_sort/include/batcher_ints_sort.hpp"

#include <omp.h>

using namespace std::chrono_literals;

std::vector<int> ChuvashovOMP_BatcherEven(std::vector<int> arr1, std::vector<int> arr2, int threads_count = 4) {
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
#pragma omp parallel for num_threads(threads_count)
    for (int l = k; l < static_cast<int>(arr2.size()); l += 2) {
      result[i] = arr2[l];
      i++;
    }
  } else {
#pragma omp parallel for num_threads(threads_count)
    for (int l = j; l < static_cast<int>(arr1.size()); l += 2) {
      result[i] = arr1[l];
      i++;
    }
  }

  return result;
}

std::vector<int> ChuvashovOMP_BatcherOdd(std::vector<int> arr1, std::vector<int> arr2, int threads_count = 4) {
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
#pragma omp parallel for num_threads(threads_count)
    for (int l = k; l < static_cast<int>(arr2.size()); l += 2) {
      result[i] = arr2[l];
      i++;
    }
  } else {
#pragma omp parallel for num_threads(threads_count)
    for (int l = j; l < static_cast<int>(arr1.size()); l += 2) {
      result[i] = arr1[l];
      i++;
    }
  }

  return result;
}

std::vector<int> ChuvashovOMP_merge(std::vector<int> arr1, std::vector<int> arr2, int threads_count = 4) {
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
#pragma omp parallel for num_threads(threads_count)
    for (int l = i; l < static_cast<int>(result.size()); l++) {
      result[l] = arr1[j];
      j++;
    }
  }
#pragma omp parallel for num_threads(threads_count)
  for (int l = 0; l < static_cast<int>(result.size() - 1); l++) {
    if (result[l] > result[l + 1]) {
      std::swap(result[l], result[l + 1]);
    }
  }

  return result;
}

std::vector<int> ChuvashovOMP_BatcherSort(const std::vector<int> &arr1, const std::vector<int> &arr2) {
  std::vector<int> even = ChuvashovOMP_BatcherEven(arr1, arr2);
  std::vector<int> odd = ChuvashovOMP_BatcherOdd(arr1, arr2);
  std::vector<int> result = ChuvashovOMP_merge(even, odd);
  if (!std::is_sorted(result.begin(), result.end())) {
    std::vector<int> left(result.begin(), result.begin() + result.size() / 2);
    std::vector<int> right(result.begin() + result.size() / 2, result.end());
    return ChuvashovOMP_BatcherSort(left, right);
  }
  return result;
}

bool Chuvashov_OMPBatcherSort::pre_processing() {
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
  return true;
}

bool Chuvashov_OMPBatcherSort::validation() {
  internal_order_test();
  return (std::is_sorted(arr1.begin(), arr1.end()) && std::is_sorted(arr2.begin(), arr2.end()));
}

bool Chuvashov_OMPBatcherSort::run() {
  internal_order_test();
  output = ChuvashovOMP_BatcherSort(arr1, arr2);
  return true;
}

bool Chuvashov_OMPBatcherSort::post_processing() {
  internal_order_test();
  std::copy(output.begin(), output.end(), reinterpret_cast<int *>(taskData->outputs[0]));
  return true;
}
