// Copyright 2024 Petrov Maksim

#include "tbb/petrov_m_double_radix_sort/include/ops_tbb.hpp"

#include <thread>
#include <vector>

using namespace std::chrono_literals;
using namespace petrov_tbb;

void PetrovRadixSortDoubleTBB::PetrovCountSort(double* in, double* out, int len, int exp) {
  auto* buf = reinterpret_cast<unsigned char*>(in);
  int count[256] = {0};
  // Counting the number of elements for each byte value in a given digit (exp)
  for (int i = 0; i < len; i++) {
    count[buf[8 * i + exp]]++;
  }
  int sum = 0;
  // Counting the sum of elements for each byte value
  for (int i = 0; i < 256; i++) {
    int temp = count[i];
    count[i] = sum;
    sum += temp;
  }
  // Placing elements on the output array according to their sort order
  for (int i = 0; i < len; i++) {
    out[count[buf[8 * i + exp]]] = in[i];
    count[buf[8 * i + exp]]++;
  }
}

bool PetrovRadixSortDoubleTBB::PetrovCountSortSigns(const double* in, double* out, int len) {
  bool positiveFlag = false;
  bool negativeFlag = false;
  // Index of the first negative number
  int firstNegativeIndex = -1;

  // Passing through the input array to determine the presence of positive and negative numbers
  for (int i = 0; i < len; i++) {
    if (positiveFlag && negativeFlag) {
      break;
    }
    // If the number is negative and the negative numbers flag is not set yet, set it and store the index of the first
    // negative number
    if (in[i] < 0 && !negativeFlag) {
      negativeFlag = true;
      firstNegativeIndex = i;
    }
    if (in[i] > 0 && !positiveFlag) {
      positiveFlag = true;
    }
  }
  // If both flags are set, we perform a special kind of sorting
  if (positiveFlag && negativeFlag) {
    bool forward = false;
    int j = len - 1;
    // We go through the input array, placing the negative numbers in reverse order, starting from the last one
    for (int i = 0; i < len; i++) {
      out[i] = in[j];
      // Moving forward or backward depending on the value of the flag
      if (forward) {
        j++;
      } else {
        j--;
      }
      // When we reach the index of the first negative number from the beginning, switch the direction
      if (j == firstNegativeIndex - 1 && !forward) {
        j = 0;
        forward = true;
      }
    }
    return true;
  }
  // If all the numbers are negative, just flip the array
  if (!positiveFlag) {
    for (int i = len - 1, j = 0; i >= 0; i--, j++) {
      out[j] = in[i];
    }
    return true;
  }
  // If all the numbers are positive, there is no need for special sorting, so we return false
  return false;
}

tbb::concurrent_vector<std::vector<double>> PetrovRadixSortDoubleTBB::PetrovSplitVector(const std::vector<double>& data,
                                                                                        int numParts) {
  tbb::concurrent_vector<std::vector<double>> resultVec;
  std::vector<double> tmp;

  if (numParts < 2 || (int)data.size() < numParts) {
    resultVec.push_back(data);
    return resultVec;
  }

  int part = data.size() / numParts;
  int remainder = data.size() % numParts;

  for (int i = 0; i < numParts; ++i) {
    int start = i * part + (((i) < (remainder)) ? (i) : (remainder));
    int end = start + part + (i < remainder ? 1 : 0);
    resultVec.emplace_back(data.begin() + start, data.begin() + end);
  }

  return resultVec;
}

std::vector<double> PetrovRadixSortDoubleTBB::PetrovMerge(const std::vector<double>& arr1,
                                                          const std::vector<double>& arr2) {
  int len1 = arr1.size();
  int len2 = arr2.size();
  std::vector<double> out;
  out.reserve(len1 + len2);
  int indexFirst = 0;
  int indexSecond = 0;
  while (indexFirst < len1 && indexSecond < len2) {
    if (arr1[indexFirst] < arr2[indexSecond]) {
      out.push_back(arr1[indexFirst++]);
    } else {
      out.push_back(arr2[indexSecond++]);
    }
  }

  while (indexFirst < len1) {
    out.push_back(arr1[indexFirst++]);
  }
  while (indexSecond < len2) {
    out.push_back(arr2[indexSecond++]);
  }

  return out;
}

std::vector<double> PetrovRadixSortDoubleTBB::PetrovRadixSort(const std::vector<double>& data) {
  int len = static_cast<int>(data.size());
  std::vector<double> in = data;
  std::vector<double> out(data.size());

  for (int i = 0; i < 4; i++) {
    PetrovCountSort(in.data(), out.data(), len, 2 * i);
    PetrovCountSort(out.data(), in.data(), len, 2 * i + 1);
  }
  if (!PetrovCountSortSigns(in.data(), out.data(), len)) {
    in.swap(out);
  }
  return out;
}

std::vector<double> PetrovRadixSortDoubleTBB::PetrovBinaryMergeTree(
    tbb::concurrent_vector<std::vector<double>>& sortedVectors) {
  while (sortedVectors.size() > 1) {
    tbb::concurrent_vector<std::vector<double>> mergedVectors;

    tbb::parallel_for(tbb::blocked_range<int>(0, static_cast<int>(sortedVectors.size() / 2)),
                      [&](const tbb::blocked_range<int>& r) {
                        for (int i = r.begin(); i < r.end(); ++i) {
                          std::vector<double> merged = PetrovMerge(sortedVectors[2 * i], sortedVectors[2 * i + 1]);
                          mergedVectors.push_back(std::move(merged));
                        }
                      });

    if (sortedVectors.size() % 2 != 0) {
      mergedVectors.push_back(std::move(sortedVectors.back()));
    }

    sortedVectors.swap(mergedVectors);
  }

  return sortedVectors.front();
}

std::vector<double> PetrovRadixSortDoubleTBB::PetrovRadixSortTbb(const std::vector<double>& data, int numParts) {
  tbb::concurrent_vector<std::vector<double>> vectorsForParallel = PetrovSplitVector(data, numParts);
  int size = static_cast<int>(vectorsForParallel.size());

  // int grainsize = (((1) > (size / numParts)) ? (1) : (size / numParts));
  tbb::parallel_for(tbb::blocked_range<int>(0, size), [&](const tbb::blocked_range<int>& r) {
    for (int i = r.begin(); i < r.end(); ++i) {
      vectorsForParallel[i] = PetrovRadixSort(vectorsForParallel[i]);
    }
  });

  return PetrovBinaryMergeTree(vectorsForParallel);
}

bool PetrovRadixSortDoubleTBB::pre_processing() {
  internal_order_test();
  try {
    data_size = taskData->inputs_count[0];
    while (!sort.empty()) {
      sort.pop_back();
    }
    auto* inp = reinterpret_cast<double*>((taskData->inputs[0]));
    for (int i = 0; i < data_size; i++) {
      sort.push_back(inp[i]);
    }
  } catch (...) {
    std::cout << "\n";
    std::cout << "Double radix sort error";
    std::cout << "\n";
    return false;
  }
  return true;
}

bool PetrovRadixSortDoubleTBB::validation() {
  internal_order_test();
  // Check count elements of output
  return ((taskData->inputs_count[0] > 1) && (taskData->outputs_count[0] == taskData->inputs_count[0]));
}

bool PetrovRadixSortDoubleTBB::run() {
  internal_order_test();
  try {
    sort = (PetrovRadixSortTbb(sort, 4));
  } catch (...) {
    std::cout << "\n";
    std::cout << "Double radix sort error";
    std::cout << "\n";
    return false;
  }
  return true;
}

bool PetrovRadixSortDoubleTBB::post_processing() {
  internal_order_test();
  try {
    auto* outputs = reinterpret_cast<double*>(taskData->outputs[0]);
    for (int i = 0; i < data_size; i++) {
      outputs[i] = sort[i];
    }
  } catch (...) {
    std::cout << "\n";
    std::cout << "Double radix sort error";
    std::cout << "\n";
    return false;
  }
  return true;
}
