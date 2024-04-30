// Copyright 2024 Kashirin Alexander

#include <oneapi/tbb.h>
#include <cmath>

#include "tbb/kashirin_a_int_radix_sort_batcher/include/ops_tbb.hpp"
using namespace std::chrono_literals;

int remainder(int num, int k) { return (num / static_cast<int>(pow(10, k - 1))) % 10; }

void oddToArr(const std::vector<int>& src, std::vector<int>& res) {
  tbb::parallel_for(tbb::blocked_range<int>(1, src.size(), 2), [&](const tbb::blocked_range<int>& range) {
    for (int i = range.begin(); i < range.end(); ++i) {
      int j = (i - 1) / 2;
      res[j] = src[i];
    }
  });
}

void evenToArr(const std::vector<int>& src, std::vector<int>& res) {
  tbb::parallel_for(tbb::blocked_range<int>(0, src.size(), 2), [&](const tbb::blocked_range<int>& range) {
    for (int i = range.begin(); i < range.end(); ++i) {
      int j = (i - 1) / 2;
      res[j] = src[i];
    }
  });
}

void radixSort(std::vector<int>& src, size_t left, size_t right) {
  std::vector<std::vector<int>> tmp(10, std::vector<int>((static_cast<int>(right - left)), 0));
  std::vector<int> amount(10, 0);
  int k = 1;
  std::vector<int> sz(10, 0); 
  while (k <= 3) {
    for (int i = (int)left; i <= (int)right; i++) {
      int rem = remainder(src[i], k);
      tmp[rem][amount[rem]++] = src[i];
    }
    for (int i = 1; i < 10; i++) sz[i] = sz[i - 1] + amount[i - 1];
    tbb::parallel_for(0, 10, [&](int n) {
      int threadNum = n;
      int start = sz[threadNum];
      int end = start + amount[threadNum];
      for (int i = start, j = 0; i < end; i++, j++) {
        src[i] = tmp[threadNum][j];
      }
      amount[threadNum] = 0;
    });
    k++;
  }
}

void merge2(std::vector<int>& src1, std::vector<int>& src2, std::vector<int>& res) {
  size_t i = 0;
  size_t j = 0;
  size_t end = res.size();
  size_t k = 0;
  while (k < end) {
    if ((i < src1.size()) && ((j >= src2.size()) || (src1[i] < src2[j]))) {
      res[k++] = src1[i++];
    } else if ((j < src2.size()) && ((i >= src1.size()) || (src1[i] > src2[j]))) {
      res[k++] = src2[j++];
    } else {
      res[k + 1] = res[k] = src1[i++];
      k += 2;
      j++;
    }
  }
}

void oddEvenMergeSort(std::vector<int>& src, std::vector<int>& res, size_t left, size_t right) {
  std::vector<int> odd(src.size() / 2 + src.size() % 2);
  std::vector<int> even(src.size() - odd.size());

  oddToArr(src, odd);
  evenToArr(src, even);

  radixSort(odd, 0, odd.size() - 1);
  radixSort(even, 0, even.size() - 1);

  merge2(odd, even, res);
}

bool TbbIntRadixSortWithBatcherMerge::pre_processing() {
  internal_order_test();
  // Init value for input and output
  input = std::vector<int>(taskData->inputs_count[0]);
  auto* tmp = reinterpret_cast<int*>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    input[i] = tmp[i];
  }
  result = std::vector<int>(taskData->outputs_count[0]);
  return true;
}

bool TbbIntRadixSortWithBatcherMerge::validation() {
  internal_order_test();

  // Check count elements of output

  return taskData->inputs_count[0] == taskData->outputs_count[0];
  // return taskData->inputs_count.size() == taskData->outputs_count.size();
  // return true;
}

bool TbbIntRadixSortWithBatcherMerge::run() {
  internal_order_test();
  try {
    oddEvenMergeSort(input, result, 0, input.size() - 1);
  } catch (...) {
    return false;
  }
  // std::this_thread::sleep_for(20ms);
  return true;
}

bool TbbIntRadixSortWithBatcherMerge::post_processing() {
  internal_order_test();
  std::copy(result.begin(), result.end(), reinterpret_cast<int*>(taskData->outputs[0]));
  return true;
  // return std::is_sorted(result.begin(), result.end());
}