// Copyright 2024 Smirnov Leonid
#include "omp/smirnov_l_radixsort_omp/include/ops_omp.hpp"

#include <omp.h>

#include <algorithm>
#include <cmath>
#include <random>
#include <thread>
#include <utility>
#include <vector>

std::vector<int> getRandomVectorSmirn(const int length) {
  std::vector<int> randomVector(length);
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < length; i++) {
    randomVector[i] = gen() % 1000;
  }
  return randomVector;
}

bool RadixSortOMPSequential::validation() {
  internal_order_test();
  // Check count elements of output
  if (taskData->inputs_count.size() != 1 || taskData->outputs_count.size() != 1 || taskData->inputs.size() != 1 ||
      taskData->outputs.size() != 1) {
    return false;
  }
  if (taskData->inputs[0] == nullptr || taskData->outputs[0] == nullptr) {
    return false;
  }
  if (taskData->inputs_count[0] != taskData->outputs_count[0]) {
    return false;
  }
  if (taskData->inputs_count[0] < 0 || taskData->outputs_count[0] < 0) {
    return false;
  }
  return true;
}

bool RadixSortOMPSequential::pre_processing() {
  internal_order_test();
  try {
    const auto size = taskData->inputs_count[0];
    auto* data = reinterpret_cast<int*>(taskData->inputs[0]);
    workVector = std::vector(data, data + size);
    return true;
  } catch (...) {
    return false;
  }
}

bool RadixSortOMPSequential::run() {
  internal_order_test();
  try {
    std::vector<int> temporaryArray = workVector;

    const int maxElem = *std::max_element(temporaryArray.begin(), temporaryArray.end());
    const int numDigitsMaxElement = maxElem == 0 ? 1 : static_cast<int>(log10(abs(maxElem))) + 1;

    std::vector<int> temp(temporaryArray.size());

    for (int digitIndex = 0; digitIndex < numDigitsMaxElement; digitIndex++) {
      int devider = static_cast<int>(pow(10, digitIndex));
      int minDigit = temporaryArray[0] % (devider * 10) / devider;
      int maxDigit = minDigit;

      for (const int num : temporaryArray) {
        int currentDigit = num % (devider * 10) / devider;
        minDigit = std::min(minDigit, currentDigit);
        maxDigit = std::max(maxDigit, currentDigit);
      }

      std::vector<int> frequency(maxDigit - minDigit + 1);
      for (const int num : temporaryArray) {
        int keyCurrentDigit = num % (devider * 10) / devider;
        frequency[keyCurrentDigit - minDigit]++;
      }

      int cumulativeSum = 0;
      for (int& c : frequency) {
        cumulativeSum += c;
        c = cumulativeSum;
      }

      for (int i = static_cast<int>(temporaryArray.size()) - 1; i >= 0; i--) {
        const int key = temporaryArray[i] % (devider * 10) / devider;
        temp[--frequency[key - minDigit]] = temporaryArray[i];
      }
      temporaryArray = temp;
    }
    workVector = temporaryArray;
    return true;
  } catch (...) {
    return false;
  }
}

bool RadixSortOMPSequential::post_processing() {
  internal_order_test();
  auto* outputs = reinterpret_cast<int*>(taskData->outputs[0]);
  for (size_t i = 0; i < workVector.size(); i++) {
    outputs[i] = workVector[i];
  }
  return true;
}

// Parallel //

bool RadixSortOMPParallelS::validation() {
  internal_order_test();

  if (taskData->inputs_count.size() != 1 || taskData->outputs_count.size() != 1 || taskData->inputs.size() != 1 ||
      taskData->outputs.size() != 1) {
    return false;
  }

  if (taskData->inputs[0] == nullptr || taskData->outputs[0] == nullptr) {
    return false;
  }

  if (taskData->inputs_count[0] != taskData->outputs_count[0]) {
    return false;
  }

  if (taskData->inputs_count[0] < 0 || taskData->outputs_count[0] < 0) {
    return false;
  }

  return true;
}

bool RadixSortOMPParallelS::pre_processing() {
  internal_order_test();
  try {
    int size = static_cast<int>(taskData->inputs_count[0]);
    auto* data = reinterpret_cast<int*>(taskData->inputs[0]);

    workVector = std::vector<int>(size, 0);

#pragma omp parallel for
    for (int i = 0; i < size; i++) {
      workVector[i] = data[i];
    }

    return true;
  } catch (...) {
    return false;
  }
}

std::vector<int> radixSortSmirn(std::vector<int> vector) {
  std::vector<int> temporaryArray = std::move(vector);
  const int maxElem = *std::max_element(temporaryArray.begin(), temporaryArray.end());

  const int numDigitsMaxElement = maxElem == 0 ? 1 : static_cast<int>(log10(abs(maxElem))) + 1;

  for (int digitIndex = 0; digitIndex < numDigitsMaxElement; digitIndex++) {
    int devider = static_cast<int>(pow(10, digitIndex));
    int minDigit = temporaryArray[0] % (devider * 10) / devider;
    int maxDigit = minDigit;

    for (const int num : temporaryArray) {
      int currentDigit = num % (devider * 10) / devider;
      minDigit = std::min(minDigit, currentDigit);
      maxDigit = std::max(maxDigit, currentDigit);
    }

    std::vector<int> frequency(maxDigit - minDigit + 1);
    for (const int num : temporaryArray) {
      int keyCurrentDigit = num % (devider * 10) / devider;
      frequency[keyCurrentDigit - minDigit]++;
    }

    int cumulativeSum = 0;
    for (int& c : frequency) {
      cumulativeSum += c;
      c = cumulativeSum;
    }

    std::vector<int> temp(temporaryArray.size());
    for (int i = static_cast<int>(temporaryArray.size()) - 1; i >= 0; i--) {
      const int key = temporaryArray[i] % (devider * 10) / devider;
      temp[--frequency[key - minDigit]] = temporaryArray[i];
    }
    temporaryArray = std::move(temp);
  }
  return temporaryArray;
}

std::vector<int> mergeListsSmirn(const std::vector<int>& firstVector, const std::vector<int>& secondVector) {
  std::vector<int> result(firstVector.size() + secondVector.size());
  std::merge(firstVector.begin(), firstVector.end(), secondVector.begin(), secondVector.end(), result.begin());
  return result;
}

bool RadixSortOMPParallelS::run() {
  internal_order_test();
  try {
    std::vector<int> resultVector;
    int sizeResultVector = workVector.size();

#pragma omp parallel shared(resultVector, sizeResultVector)
    {
      int currentThread = omp_get_thread_num();
      int threadNum = omp_get_num_threads();

      if (threadNum >= sizeResultVector) {
        threadNum = sizeResultVector;
      }

      if (currentThread < sizeResultVector) {
        int step = sizeResultVector / threadNum;
        int left = step * currentThread;
        int righ = step * (currentThread + 1);
        if (currentThread == threadNum - 1) {
          righ += sizeResultVector - step * threadNum;
        }

        std::vector<int> workVectorLocal(workVector.begin() + left, workVector.begin() + righ);

        workVectorLocal = radixSortSmirn(workVectorLocal);

#pragma omp critical
        { resultVector = mergeListsSmirn(resultVector, workVectorLocal); }
      }
    }
    workVector = resultVector;
    return true;
  } catch (...) {
    return false;
  }
}

bool RadixSortOMPParallelS::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < workVector.size(); i++) {
    reinterpret_cast<int*>(taskData->outputs[0])[i] = workVector[i];
  }
  return true;
}
