// Copyright 2024 Balyasov Ivan
#include "tbb/balyasov_i_sort_simple_merge_tbb/include/ops_tbb.hpp"

#include <tbb/tbb.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

bool RadixSortClassTaskTbb::pre_processing() {
  internal_order_test();
  try {
    int* intData = reinterpret_cast<int*>(taskData->inputs[0]);
    SizeVector = taskData->inputs_count[0];
    VectorSort.resize(SizeVector);
    std::copy(intData, intData + SizeVector, VectorSort.begin());

  } catch (...) {
    return false;
  }
  return true;
}

bool RadixSortClassTaskTbb::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->outputs_count[0] && taskData->outputs[0] != nullptr &&
         taskData->inputs[0] != nullptr && taskData->inputs_count[0] != 0;
}

std::vector<int> RadixSortClassClist::radixSort(std::vector<int> vec) {
  std::vector<int> tempVec;
  tempVec = vec;

  int numDev = 0;
  int maxNum = 0;
  int minKey;
  int maxKey;

  for (size_t j = 0; j < tempVec.size(); j++) {
    if (maxNum <= tempVec[j]) {
      maxNum = tempVec[j];
    }
  }

  numDev = maxNum == 0 ? 1 : log10(abs(maxNum)) + 1;

  for (int dev = 0; dev < numDev; dev++) {
    int devider = pow(10, dev);

    minKey = tempVec[0] % (devider * 10) / devider;

    maxKey = minKey;

    for (size_t j = 0; j < tempVec.size(); j++) {
      int digit = tempVec[j] % (devider * 10) / devider;
      if (digit < minKey) {
        minKey = digit;
      }
      if (digit > maxKey) {
        maxKey = digit;
      }
    }

    std::vector<int> count(maxKey - minKey + 1);
    for (size_t j = 0; j < tempVec.size(); j++) {
      count[(tempVec[j] % (devider * 10) / devider) - minKey]++;
    }

    int size = tempVec.size();
    for (int j = count.size() - 1; j >= 0; j--) {
      size -= count[j];
      count[j] = size;
    }

    std::vector<int> temp(tempVec.size());

    for (size_t j = 0; j < tempVec.size(); j++) {
      temp[count[(tempVec[j] % (devider * 10) / devider) - minKey]] = tempVec[j];
      count[(tempVec[j] % (devider * 10) / devider) - minKey]++;
    }

    for (size_t j = 0; j < tempVec.size(); j++) {
      tempVec[j] = temp[j];
    }
  }
  vec = tempVec;
  return vec;
}

std::vector<int> RadixSortClassClist::mergeVector(std::vector<int>* vec1, std::vector<int>* vec2) {
  std::vector<int> result(vec1->size() + vec2->size());
  std::merge((*vec1).begin(), (*vec1).end(), (*vec2).begin(), (*vec2).end(), result.begin());
  return result;
}

bool RadixSortClassTaskTbb::run() {
  internal_order_test();

  try {
    RadixSortClassClist clist;
    tbb::task_arena arena;

    int numThreads = arena.max_concurrency();
    int step = VectorSort.size() / numThreads;
    if (step < 1) {
      step = 1;
    }

    tbb::parallel_reduce(tbb::blocked_range<std::vector<int>::iterator>(VectorSort.begin(), VectorSort.end(), step),
                         clist);

    VectorSort = clist.VectorResult;
  } catch (...) {
    return false;
  }
  return true;
}

bool RadixSortClassTaskTbb::post_processing() {
  internal_order_test();

  for (size_t j = 0; j < VectorSort.size(); j++) reinterpret_cast<int*>(taskData->outputs[0])[j] = VectorSort[j];

  return true;
}