// Copyright 2024 Smirnov Leonid

#include "tbb/smirnov_l_tbb_radix_sort/include/ops_tbb.hpp"

#include <algorithm>
#include <cmath>
#include <random>
#include <thread>
#include <vector>

std::vector<int> getRandomVectorSmirnov(const int length) {
  std::vector<int> randomVector(length);
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < length; i++) {
    randomVector[i] = gen() % 1000;
  }
  return randomVector;
}

bool RadixSortTBBMethod::pre_processing() {
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

bool RadixSortTBBMethod::validation() {
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

bool RadixSortTBBMethod::run() {
  internal_order_test();
  try {
    RadixSortTBBBody body;

    tbb::task_arena arena;
    int num_threads = arena.max_concurrency();
    int step = workVector.size() / num_threads;
    if (step < 1) step = 1;

    tbb::parallel_reduce(tbb::blocked_range<std::vector<int>::iterator>(workVector.begin(), workVector.end(), step),
                         body);
    workVector = body.workVectorResult;
  } catch (...) {
    return false;
  }
  return true;
}

bool RadixSortTBBMethod::post_processing() {
  internal_order_test();
  try {
    for (size_t i = 0; i < workVector.size(); i++) {
      reinterpret_cast<int*>(taskData->outputs[0])[i] = workVector[i];
    }
  } catch (...) {
    return false;
  }
  return true;
}

std::vector<int> RadixSortTBBBody::radixSortSmirnov(std::vector<int> vector) {
  std::vector<int> temporaryArray = vector;

  const int maxElem = *std::max_element(temporaryArray.begin(), temporaryArray.end());
  const int numDigitsMaxElement = maxElem == 0 ? 1 : static_cast<int>(log10(abs(maxElem))) + 1;

  std::vector<int> temp(temporaryArray.size());

  for (int digitIndex = 0; digitIndex < numDigitsMaxElement; digitIndex++) {
    int devider = static_cast<int>(pow(10, digitIndex));
    int minDigit = temporaryArray[0] % (devider * 10) / devider;
    int maxDigit = minDigit;

    for (size_t i = 0; i < temporaryArray.size(); i++) {
      int digit = temporaryArray[i] % (devider * 10) / devider;
      if (digit < minDigit) {
        minDigit = digit;
      }
      if (digit > maxDigit) {
        maxDigit = digit;
      }
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
  vector = temporaryArray;
  return vector;
}

std::vector<int> RadixSortTBBBody::mergeListsSmirnov(const std::vector<int>& firstVector,
                                                     const std::vector<int>& secondVector) {
  std::vector<int> result(firstVector.size() + secondVector.size());
  std::merge(firstVector.begin(), firstVector.end(), secondVector.begin(), secondVector.end(), result.begin());
  return result;
}