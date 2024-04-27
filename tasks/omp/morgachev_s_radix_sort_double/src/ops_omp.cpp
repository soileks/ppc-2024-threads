// Copyright 2024 Morgachev Stepan
#include "omp/morgachev_s_radix_sort_double/include/ops_omp.hpp"

#include <algorithm>
#include <thread>

using namespace std::chrono_literals;

bool morgachev_omp::RadixSortDoubleOMP::pre_processing() {
  internal_order_test();
  try {
    dataSize = taskData->inputs_count[0];
    auto* ptrToData = reinterpret_cast<double*>(taskData->inputs[0]);
    data.resize(dataSize);
    std::copy(ptrToData, ptrToData + dataSize, data.begin());
  } catch (...) {
    return false;
  }
  return true;
}

bool morgachev_omp::RadixSortDoubleOMP::validation() {
  internal_order_test();
  return taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr && taskData->inputs_count[0] != 0 &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool morgachev_omp::RadixSortDoubleOMP::run() {
  internal_order_test();
  try {
    radixSortOMP(data, dataSize);
  } catch (...) {
    return false;
  }
  return true;
}

bool morgachev_omp::RadixSortDoubleOMP::post_processing() {
  internal_order_test();
  try {
    for (size_t i = 0; i < data.size(); i++) {
      reinterpret_cast<double*>(taskData->outputs[0])[i] = data[i];
    }
  } catch (...) {
    return false;
  }
  return true;
}

void morgachev_omp::countSort(double* in, double* out, int length, int exp) {
  auto* buffer = reinterpret_cast<unsigned char*>(in);
  int counter[256] = {0};
  int minValue = 0;
  int minPos = 0;

  for (int i = 0; i < length; i++) {
    counter[buffer[8 * i + exp]]++;
  }

  for (int j = 0; j < 256; j++) {
    if (counter[j] != 0) {
      minValue = counter[j];
      counter[j] = 0;
      minPos = ++j;
      break;
    }
  }

  for (; minPos < 256; minPos++) {
    int temp = counter[minPos];
    counter[minPos] = minValue;
    minValue += temp;
  }

  for (int i = 0; i < length; i++) {
    out[counter[buffer[8 * i + exp]]] = in[i];
    counter[buffer[8 * i + exp]]++;
  }
}

void morgachev_omp::radixSortOMP(std::vector<double>& data, size_t dataSize) {
  std::vector<double> result;

#pragma omp parallel
{
    int nThreads = omp_get_num_threads();
    int curThread = omp_get_thread_num();
    int localSize = dataSize / nThreads;
    std::vector<double> localVector;

    if (curThread == nThreads - 1) {
        std::copy(data.begin() + localSize * curThread, data.end(),
            std::back_inserter(localVector));
    } else {
        std::copy(data.begin() + localSize * curThread, data.begin() + localSize * (curThread + 1),
                std::back_inserter(localVector));
    }

    localVector = radixSortSeq(localVector);
#pragma omp critical
    result = doubleMerge(result, localVector);
}

  data = result;
}

std::vector<double> morgachev_omp::radixSortSeq(std::vector<double>& data) {
  std::vector<double> tempLeft = data;
  std::vector<double> tempRight(data.size());

  for (int i = 0; i < 4; i++) {
    countSort(tempLeft.data(), tempRight.data(), data.size(), 2 * i);
    countSort(tempRight.data(), tempLeft.data(), data.size(), 2 * i + 1);
  }

  return tempLeft;
}

std::vector<double> morgachev_omp::doubleMerge(const std::vector<double>& left,
    const std::vector<double>& right) {
  std::vector<double> result;
  result.resize(left.size() + right.size());
  std::merge(left.begin(), left.end(), right.begin(), right.end(), result.begin());
  return result;
}
