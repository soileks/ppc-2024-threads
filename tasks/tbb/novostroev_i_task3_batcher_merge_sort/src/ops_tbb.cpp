// Copyright 2024 Novostroev Ivan

#include "tbb/novostroev_i_task3_batcher_merge_sort/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

using namespace std::chrono_literals;

std::mutex idx_mutex;

std::vector<int> mergeElements(std::vector<int>& vec0, std::vector<int>& vec1, std::vector<int>& vec2, size_t idx1,
                               size_t idx2, size_t increment, int modification) {
  std::vector<int> mergedVec = vec0;
  size_t idx = 0;

  if ((modification == 0) || (modification == 1)) {
    while ((idx1 < vec1.size()) && (idx2 < vec2.size())) {
      if (vec1[idx1] <= vec2[idx2]) {
        mergedVec[idx] = vec1[idx1];
        idx1 += increment;
      } else {
        mergedVec[idx] = vec2[idx2];
        idx2 += increment;
      }
      idx++;
    }

    return mergedVec;
    if (idx1 >= vec1.size()) {
      tbb::parallel_for(
          tbb::blocked_range<size_t>(idx2, vec2.size()),
          [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); i += increment) {
              std::lock_guard<std::mutex> guard(idx_mutex);
              if (idx < mergedVec.size()) {
                mergedVec[idx] = vec2[i];
                idx++;
              }
            }
          },
          tbb::simple_partitioner());
    } else {
      tbb::parallel_for(
          tbb::blocked_range<size_t>(idx1, vec1.size()),
          [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); i += increment) {
              std::lock_guard<std::mutex> guard(idx_mutex);
              if (idx < mergedVec.size()) {
                mergedVec[idx] = vec1[i];
                idx++;
              }
            }
          },
          tbb::simple_partitioner());
    }
  } else {
    while ((idx1 < vec1.size()) && (idx2 < vec2.size())) {
      mergedVec[idx] = vec1[idx1];
      if (idx + 1 < mergedVec.size()) {
        mergedVec[idx + 1] = vec2[idx2];
      }
      idx += 2;
      idx1++;
      idx2++;
    }

    if ((idx2 >= vec2.size()) && (idx1 < vec1.size())) {
      tbb::parallel_for(
          tbb::blocked_range<size_t>(idx, mergedVec.size()),
          [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); i += increment) {
              std::lock_guard<std::mutex> guard(idx_mutex);
              if (idx1 < vec1.size()) {
                mergedVec[i] = vec1[idx1];
                idx1++;
              }
            }
          },
          tbb::simple_partitioner());
    }

    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, mergedVec.size() - 1),
        [&](const tbb::blocked_range<size_t>& range) {
          for (size_t i = range.begin(); i != range.end(); i += increment) {
            if (i + 1 < mergedVec.size() && mergedVec[i] > mergedVec[i + 1]) {
              std::lock_guard<std::mutex> guard(idx_mutex);
              std::swap(mergedVec[i], mergedVec[i + 1]);
            }
          }
        },
        tbb::simple_partitioner());
  }

  return mergedVec;
}
std::vector<int> getOddElements(std::vector<int> vec1, std::vector<int> vec2, int threads_count = 4) {
  size_t idx1 = 1;
  size_t idx2 = 1;
  std::vector<int> mergedVec(vec1.size() / 2 + vec2.size() / 2);
  return mergeElements(mergedVec, vec1, vec2, idx1, idx2, 2, 0);
}

std::vector<int> getEvenElements(std::vector<int> vec1, std::vector<int> vec2, int threads_count = 4) {
  size_t idx1 = 0;
  size_t idx2 = 0;
  std::vector<int> mergedVec(vec1.size() / 2 + vec2.size() / 2 + vec1.size() % 2 + vec2.size() % 2);
  return mergeElements(mergedVec, vec1, vec2, idx1, idx2, 2, 1);
}

std::vector<int> mergeVectors(std::vector<int> vec1, std::vector<int> vec2, int threads_count = 4) {
  size_t idx1 = 0;
  size_t idx2 = 0;
  std::vector<int> mergedVec(vec1.size() + vec2.size());
  return mergeElements(mergedVec, vec1, vec2, idx1, idx2, 1, 2);
}

std::vector<int> batcherSort(const std::vector<int>& vec1, const std::vector<int>& vec2) {
  std::vector<int> oddElements = getOddElements(vec1, vec2);
  std::vector<int> evenElements = getEvenElements(vec1, vec2);
  std::vector<int> mergedVec = mergeVectors(evenElements, oddElements);

  if (!is_sorted(mergedVec.begin(), mergedVec.end())) {
    std::vector<int> leftHalf(mergedVec.begin(), mergedVec.begin() + mergedVec.size() / 2);
    std::vector<int> rightHalf(mergedVec.begin() + mergedVec.size() / 2, mergedVec.end());
    return batcherSort(leftHalf, rightHalf);
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

bool BatcherMergeSortTBB::pre_processing() {
  internal_order_test();
  in = std::vector<int>(taskData->inputs_count[0]);
  auto* tmp_ptr_A = reinterpret_cast<int*>(taskData->inputs[0]);
  for (size_t idx = 0; idx < taskData->inputs_count[0]; idx++) {
    in[idx] = tmp_ptr_A[idx];
  }
  prepareInput(in, vec1, vec2);
  return true;
}

bool BatcherMergeSortTBB::validation() {
  internal_order_test();
  return validateSort(vec1, vec2);
}

bool BatcherMergeSortTBB::run() {
  internal_order_test();
  out = runBatcherSort(vec1, vec2);
  return true;
}

bool BatcherMergeSortTBB::post_processing() {
  internal_order_test();
  copyOutput(out, reinterpret_cast<int*>(taskData->outputs[0]));
  return true;
}
