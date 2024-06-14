// Copyright 2024 Novostroev Ivan

#include "stl/novostroev_i_task4_batcher_merge_sort/include/ops_stl.hpp"

using namespace std::chrono_literals;

std::mutex idx_mutex;

std::vector<int> mergeElements(std::vector<int>& vec0, std::vector<int>& vec1, std::vector<int>& vec2, size_t idx1,
                               size_t idx2, size_t increment, int modification) {
  std::vector<int> mergedVec = vec0;
  size_t idx = 0;

  if ((modification == 0) || (modification == 1)) {
    size_t len1 = vec1.size();
    size_t len2 = vec2.size();

    auto merge1 = [&] {
      while ((idx1 < len1) && (idx2 < len2)) {
        if (vec1[idx1] <= vec2[idx2]) {
          if (idx < mergedVec.size()) {
            mergedVec[idx] = vec1[idx1];
            idx1 += increment;
          }
        } else {
          if (idx < mergedVec.size()) {
            mergedVec[idx] = vec2[idx2];
            idx2 += increment;
          }
        }
        idx++;
      }
    };

    auto merge2 = [&] {
      while (idx1 < len1) {
        if (idx < mergedVec.size()) {
          mergedVec[idx] = vec1[idx1];
          idx1 += increment;
          idx++;
        }
      }
    };

    auto merge3 = [&] {
      while (idx2 < len2) {
        if (idx < mergedVec.size()) {
          mergedVec[idx] = vec2[idx2];
          idx2 += increment;
          idx++;
        }
      }
    };

    std::thread t1(merge1);
    std::thread t2(merge2);
    std::thread t3(merge3);

    t1.join();
    t2.join();
    t3.join();

    return mergedVec;
  }

  return mergedVec;
}

std::vector<int> getOddElements(std::vector<int> vec1, std::vector<int> vec2) {
  size_t idx1 = 1;
  size_t idx2 = 1;
  std::vector<int> mergedVec(vec1.size() / 2 + vec2.size() / 2);
  return mergeElements(mergedVec, vec1, vec2, idx1, idx2, 2, 0);
}

std::vector<int> getEvenElements(std::vector<int> vec1, std::vector<int> vec2) {
  size_t idx1 = 0;
  size_t idx2 = 0;
  std::vector<int> mergedVec(vec1.size() / 2 + vec2.size() / 2 + vec1.size() % 2 + vec2.size() % 2);
  return mergeElements(mergedVec, vec1, vec2, idx1, idx2, 2, 1);
}

std::vector<int> mergeVectors(std::vector<int> vec1, std::vector<int> vec2) {
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

bool BatcherMergeSortSTL::pre_processing() {
  internal_order_test();
  in = std::vector<int>(taskData->inputs_count[0]);
  auto* tmp_ptr_A = reinterpret_cast<int*>(taskData->inputs[0]);
  for (size_t idx = 0; idx < taskData->inputs_count[0]; idx++) {
    in[idx] = tmp_ptr_A[idx];
  }
  prepareInput(in, vec1, vec2);
  return true;
}

bool BatcherMergeSortSTL::validation() {
  internal_order_test();
  return validateSort(vec1, vec2);
}

bool BatcherMergeSortSTL::run() {
  internal_order_test();
  out = runBatcherSort(vec1, vec2);
  return true;
}

bool BatcherMergeSortSTL::post_processing() {
  internal_order_test();
  copyOutput(out, reinterpret_cast<int*>(taskData->outputs[0]));
  return true;
}
