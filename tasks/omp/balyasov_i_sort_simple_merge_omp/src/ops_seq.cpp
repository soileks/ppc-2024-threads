// Copyright 2024 Balyasov Ivan
#include "omp/balyasov_i_sort_simple_merge_omp/include/ops_seq.hpp"

#include <omp.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

bool RadixSortTaskOMP::pre_processing() {
  internal_order_test();

  try {
    int* int_data = reinterpret_cast<int*>(taskData->inputs[0]);
    SizeVector = taskData->inputs_count[0];
    VectorForSort.resize(SizeVector);
    std::copy(int_data, int_data + SizeVector, VectorForSort.begin());

  } catch (...) {
    return false;
  }

  return true;
}

bool RadixSortTaskOMP::validation() {
  internal_order_test();

  return taskData->inputs_count[0] == taskData->outputs_count[0] && taskData->outputs[0] != nullptr &&
         taskData->inputs[0] != nullptr && taskData->inputs_count[0] != 0;
}

std::vector<int> radixSort(std::vector<int> vec) {
  std::vector<int> tempVec;
  tempVec = vec;
  int numDev, maxNum = 0;
  int minKey, maxKey;

  for (size_t i = 0; i < tempVec.size(); i++) {
    if (maxNum <= tempVec[i]) {
      maxNum = tempVec[i];
    }
  }

  if (maxNum == 0) {
    numDev = 1;
  } else
    numDev = log10(abs(maxNum)) + 1;

  for (int dev = 0; dev < numDev; dev++) {
    int devider = pow(10, dev);

    minKey = tempVec[0] % (devider * 10) / devider;

    maxKey = minKey;

    for (size_t i = 0; i < tempVec.size(); i++) {
      int digit = tempVec[i] % (devider * 10) / devider;
      if (digit < minKey) {
        minKey = digit;
      }
      if (digit > maxKey) {
        maxKey = digit;
      }
    }

    std::vector<int> count(maxKey - minKey + 1);
    for (size_t i = 0; i < tempVec.size(); i++) {
      count[(tempVec[i] % (devider * 10) / devider) - minKey]++;
    }

    int size = tempVec.size();
    for (int i = count.size() - 1; i >= 0; i--) {
      size -= count[i];
      count[i] = size;
    }
    std::vector<int> temp(tempVec.size());
    for (size_t i = 0; i < tempVec.size(); i++) {
      temp[count[(tempVec[i] % (devider * 10) / devider) - minKey]] = tempVec[i];
      count[(tempVec[i] % (devider * 10) / devider) - minKey]++;
    }
    for (size_t i = 0; i < tempVec.size(); i++) {
      tempVec[i] = temp[i];
    }
  }
  vec = tempVec;
  return vec;
}

std::vector<int> mergeVector(std::vector<int>* vec1, std::vector<int>* vec2) {
  std::vector<int> result(vec1->size() + vec2->size());
  std::merge((*vec1).begin(), (*vec1).end(), (*vec2).begin(), (*vec2).end(), result.begin());
  return result;
}

bool RadixSortTaskOMP::run() {
  internal_order_test();
  try {
    std::vector<int> result;
    int VectorSize = VectorForSort.size();

#pragma omp parallel shared(result, VectorSize)
    {
      int currentThread = omp_get_thread_num();
      int threadNum = omp_get_num_threads();

      if (threadNum >= VectorSize) {
        threadNum = VectorSize;
      }

      if (currentThread < VectorSize) {
        int step = VectorSize / threadNum;
        int left = step * currentThread;
        int righ = step * (currentThread + 1);
        if (currentThread == threadNum - 1) {
          righ += VectorSize - step * threadNum;
        }

        std::vector<int> VectorForSortLocal(VectorForSort.begin() + left, VectorForSort.begin() + righ);

        VectorForSortLocal = radixSort(VectorForSortLocal);

#pragma omp critical
        { result = mergeVector(&result, &VectorForSortLocal); }
      }
    }
    VectorForSort = result;
  } catch (...) {
    return false;
  }
  return true;
}

bool RadixSortTaskOMP::post_processing() {
  internal_order_test();

  for (size_t i = 0; i < VectorForSort.size(); i++) reinterpret_cast<int*>(taskData->outputs[0])[i] = VectorForSort[i];

  return true;
}