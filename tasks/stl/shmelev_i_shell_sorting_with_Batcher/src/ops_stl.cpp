// Copyright 2024 Shmelev Ivan

#include "stl/shmelev_i_shell_sorting_with_Batcher/include/ops_stl.hpp"

#include <algorithm>

bool shmelev_stl::ShmelevTaskSTL::pre_processing() {
  internal_order_test();
  in = *reinterpret_cast<std::vector<int>*>(taskData->inputs[0]);
  result = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
  return true;
}

bool shmelev_stl::ShmelevTaskSTL::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool shmelev_stl::ShmelevTaskSTL::run() {
  internal_order_test();
  parallelBatcherMerge(0, in.size() - 1, 0);
  return true;
}

bool shmelev_stl::ShmelevTaskSTL::post_processing() {
  internal_order_test();
  *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]) = in;
  return true;
}

std::vector<int> shmelev_stl::ShmelevTaskSTL::generate_random_vector(int size, int min, int max) {
  std::random_device rnd_device;
  std::mt19937 mersenne_engine{rnd_device()};
  std::uniform_int_distribution<int> dist{min, max};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

  std::vector<int> vector(size);
  generate(begin(vector), end(vector), gen);
  return vector;
}

void shmelev_stl::ShmelevTaskSTL::sortingShell() {
  int n = in.size();
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      int temp = in[i];
      int j;
      for (j = i; j >= gap && in[j - gap] > temp; j -= gap) {
        in[j] = in[j - gap];
      }
      in[j] = temp;
    }
  }
}

void shmelev_stl::ShmelevTaskSTL::batcherMerge(int l, int r) {
  sortingShell();
  if (r > l) {
    int m = l + (r - l) / 2;
    for (int i = l; i <= m; i += 2) {
      if (i + 1 <= m) {
        if (in[i] > in[i + 1]) {
          std::swap(in[i], in[i + 1]);
        }
      }
    }
    batcherMerge(l, m);
    batcherMerge(m + 1, r);
    merge(l, m, r);
  }
}

bool shmelev_stl::ShmelevTaskSTL::sorted(std::vector<int> input) { return std::is_sorted(input.begin(), input.end()); }

void shmelev_stl::ShmelevTaskSTL::merge(int l, int m, int r) {
  int n1 = m - l + 1, n2 = r - m;

  std::vector<int> L(n1);
  std::vector<int> R(n2);

  for (int i = 0; i < n1; i++) {
    L[i] = in[l + i];
  }
  for (int j = 0; j < n2; j++) {
    R[j] = in[m + 1 + j];
  }

  int i = 0, j = 0, k = l;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      in[k] = L[i];
      i++;
    } else if (L[i] > R[j]) {
      in[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    in[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    in[k] = R[j];
    j++;
    k++;
  }
}

void shmelev_stl::ShmelevTaskSTL::parallelBatcherMerge(int l, int r, int depth) {
  const int max_depth = 4;
  if (l < r) {
    if (depth > max_depth) {
      batcherMerge(l, r);
    } else {
      int m = l + (r - l) / 2;
      std::thread left_thread(&shmelev_stl::ShmelevTaskSTL::parallelBatcherMerge, this, l, m, depth + 1);
      std::thread right_thread(&shmelev_stl::ShmelevTaskSTL::parallelBatcherMerge, this, m + 1, r, depth + 1);
      left_thread.join();
      right_thread.join();
      merge(l, m, r);
    }
  }
}