// Copyright 2024 Novostroev Ivan

#include "omp/novostroev_i_task2_batcher_merge_sort/include/ops_omp.hpp"

#include <omp.h>

using namespace std::chrono_literals;

std::vector<int> mergeElements(std::vector<int>& vec1, std::vector<int>& vec2, size_t& idx1, size_t& idx2, size_t increment,
                          int threads_count) {
  std::vector<int> mergedVec(vec1.size() / 2 + vec2.size() / 2 + vec1.size() % 2 + vec2.size() % 2);
  size_t idx = 0;

  while (idx1 < vec1.size() && idx2 < vec2.size()) {
    mergedVec[idx++] = (vec1[idx1] <= vec2[idx2]) ? vec1[idx1] : vec2[idx2];
    idx1 += increment;
    idx2 += increment;
  }

  if (idx1 >= vec1.size()) {
#pragma omp parallel for num_threads(threads_count)
    for (int i = idx2; i < static_cast<int>(vec2.size()); i += increment) {
      mergedVec[idx++] = vec2[i];
    }
  } else {
#pragma omp parallel for num_threads(threads_count)
    for (int i = idx1; i < static_cast<int>(vec1.size()); i += increment) {
      mergedVec[idx++] = vec1[i];
    }
  }

  return mergedVec;
}

std::vector<int> getOddElements(std::vector<int> vec1, std::vector<int> vec2, int threads_count = 4) {
  size_t idx1 = 1, idx2 = 1;
  return mergeElements(vec1, vec2, idx1, idx2, 2, threads_count);
}

std::vector<int> getEvenElements(std::vector<int> vec1, std::vector<int> vec2, int threads_count = 4) {
  size_t idx1 = 0, idx2 = 0;
  return mergeElements(vec1, vec2, idx1, idx2, 2, threads_count);
}

std::vector<int> mergeVectors(std::vector<int> vec1, std::vector<int> vec2, int threads_count = 4) {
  size_t idx1 = 0, idx2 = 0;
  return mergeElements(vec1, vec2, idx1, idx2, 1, threads_count);
}

std::vector<int> sortBatcher(const std::vector<int>& leftHalf, const std::vector<int>& rightHalf) {
  std::vector<int> mergedVec(leftHalf.size() + rightHalf.size());
  merge(leftHalf.begin(), leftHalf.end(), rightHalf.begin(), rightHalf.end(), mergedVec.begin());
  return mergedVec;
}

std::vector<int> batcherSort(const std::vector<int>& vec1, const std::vector<int>& vec2) {
  std::vector<int> oddElements = getOddElements(vec1, vec2);
  std::vector<int> evenElements = getEvenElements(vec1, vec2);
  std::vector<int> mergedVec = mergeVectors(evenElements, oddElements);

  if (!is_sorted(mergedVec.begin(), mergedVec.end())) {
    std::vector<int> leftHalf(mergedVec.begin(), mergedVec.begin() + mergedVec.size() / 2);
    std::vector<int> rightHalf(mergedVec.begin() + mergedVec.size() / 2, mergedVec.end());
    return sortBatcher(leftHalf, rightHalf);
  }

  return mergedVec;
}

std::vector<int> prepareInput(std::vector<int>& in, std::vector<int>& vec1, std::vector<int>& vec2) {
  vec1.resize(in.size() / 2);
  vec2.resize(in.size() / 2);

  for (size_t idx = 0; idx < (in.size() / 2); idx++) {
    vec1[idx] = in[idx];
    vec2[idx] = in[(in.size() / 2) + idx];
  }
  return in;
}

bool validateSort(std::vector<int>& vec1, std::vector<int>& vec2) {
  return (is_sorted(vec1.begin(), vec1.end()) && is_sorted(vec2.begin(), vec2.end()));
}

std::vector<int> runBatcherSort(std::vector<int>& vec1, std::vector<int>& vec2) { return batcherSort(vec1, vec2); }

void copyOutput(std::vector<int>& out, int* taskDataOutput) {
  copy(out.begin(), out.end(), reinterpret_cast<int*>(taskDataOutput));
}

bool BatcherMergeSortOMP::pre_processing() {
  internal_order_test();
  in = std::vector<int>(taskData->inputs_count[0]);
  auto* tmp_ptr_A = reinterpret_cast<int*>(taskData->inputs[0]);
  for (size_t idx = 0; idx < taskData->inputs_count[0]; idx++) {
    in[idx] = tmp_ptr_A[idx];
  }
  prepareInput(in, vec1, vec2);
  return true;
}

bool BatcherMergeSortOMP::validation() {
  internal_order_test();
  return validateSort(vec1, vec2);
}

bool BatcherMergeSortOMP::run() {
  internal_order_test();
  out = runBatcherSort(vec1, vec2);
  return true;
}

bool BatcherMergeSortOMP::post_processing() {
  internal_order_test();
  copyOutput(out, taskData->outputs[0]);
  return true;
}
