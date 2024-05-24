#include "tbb/morgachev_s_radix_sort_double/include/ops_tbb.hpp"

#include <tbb/tbb.h>

#include <algorithm>

using namespace std::chrono_literals;

bool morgachev_tbb::RadixSortDoubleTBB::pre_processing() {
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

bool morgachev_tbb::RadixSortDoubleTBB::validation() {
  internal_order_test();
  return taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr && taskData->inputs_count[0] != 0 &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool morgachev_tbb::RadixSortDoubleTBB::run() {
  internal_order_test();
  try {
    radixSortTBB(data, dataSize);
  } catch (...) {
    return false;
  }
  return true;
}

bool morgachev_tbb::RadixSortDoubleTBB::post_processing() {
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

void morgachev_tbb::countSort(double* in, double* out, int length, int exp) {
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

void morgachev_tbb::radixSortTBB(std::vector<double>& data, size_t dataSize) {
  std::vector<double> result;
  int nThreads = tbb::this_task_arena::max_concurrency();
  std::vector<std::vector<double>> localVectors(nThreads);

  tbb::parallel_for(tbb::blocked_range<size_t>(0, nThreads), [&](const tbb::blocked_range<size_t>& r) {
    for (size_t curThread = r.begin(); curThread != r.end(); ++curThread) {
      int localSize = dataSize / nThreads;

      if (curThread == nThreads - 1) {
        localVectors[curThread].assign(data.begin() + localSize * curThread, data.end());
      } else {
        localVectors[curThread].assign(data.begin() + localSize * curThread,
                                       data.begin() + localSize * (curThread + 1));
      }

      localVectors[curThread] = radixSortSeq(localVectors[curThread]);
    }
  });

  result = localVectors[0];
  for (size_t i = 1; i < nThreads; ++i) {
    result = doubleMerge(result, localVectors[i]);
  }

  data = result;
}

std::vector<double> morgachev_tbb::radixSortSeq(std::vector<double>& data) {
  std::vector<double> tempLeft = data;
  std::vector<double> tempRight(data.size());

  for (int i = 0; i < 4; i++) {
    countSort(tempLeft.data(), tempRight.data(), data.size(), 2 * i);
    countSort(tempRight.data(), tempLeft.data(), data.size(), 2 * i + 1);
  }

  return tempLeft;
}

std::vector<double> morgachev_tbb::doubleMerge(const std::vector<double>& left, const std::vector<double>& right) {
  std::vector<double> result;
  result.resize(left.size() + right.size());
  std::merge(left.begin(), left.end(), right.begin(), right.end(), result.begin());
  return result;
}
