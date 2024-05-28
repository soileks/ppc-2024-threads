// Copyright 2024 Shmelev Ivan

#include "stl/shmelev_i_shell_sorting_with_Batcher/include/ops_stl.hpp"

#include <algorithm>

std::vector<int> shmelev_stl::create_random_sequence(int size, int min, int max) {
  std::random_device rnd_device;
  std::mt19937 mersenne_engine{rnd_device()};
  std::uniform_int_distribution<int> dist{min, max};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

  std::vector<int> vector(size);
  generate(begin(vector), end(vector), gen);

  return vector;
}

bool shmelev_stl::is_sorted(std::vector<int> input) { return std::is_sorted(input.begin(), input.end()); }

bool shmelev_stl::ShmelevTaskSequential::pre_processing() {
  internal_order_test();
  sequence = *reinterpret_cast<std::vector<int>*>(taskData->inputs[0]);
  result = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
  return true;
}

bool shmelev_stl::ShmelevTaskSequential::validation() {
  internal_order_test();
  return taskData->outputs_count[0] == taskData->inputs_count[0];
}

bool shmelev_stl::ShmelevTaskSequential::run() {
  internal_order_test();
  batcherMergeSort(0, sequence.size() - 1);
  return true;
}

bool shmelev_stl::ShmelevTaskSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<std::vector<int>*>(taskData->outputs[0])->operator=(sequence);

  return true;
}

void shmelev_stl::ShmelevTaskSequential::shellSort() {
  int n = sequence.size();
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      int temp = sequence[i];
      int j;
      for (j = i; j >= gap && sequence[j - gap] > temp; j -= gap) {
        sequence[j] = sequence[j - gap];
      }
      sequence[j] = temp;
    }
  }
}

void shmelev_stl::ShmelevTaskSequential::batcherMergeSort(int left, int right) {
  shellSort();
  if (right > left) {
    int middle = left + (right - left) / 2;
    for (int i = left; i <= middle; i += 2) {
      if (i + 1 <= middle) {
        if (sequence[i] > sequence[i + 1]) {
          std::swap(sequence[i], sequence[i + 1]);
        }
      }
    }
    batcherMergeSort(left, middle);
    batcherMergeSort(middle + 1, right);
    mergeSequences(left, middle, right);
  }
}

void shmelev_stl::ShmelevTaskSequential::mergeSequences(int left, int middle, int right) {
  int size_left = middle - left + 1;
  int size_right = right - middle;

  std::vector<int> left_array(size_left);
  std::vector<int> right_array(size_right);

  for (int i = 0; i < size_left; ++i) {
    left_array[i] = sequence[left + i];
  }
  for (int j = 0; j < size_right; ++j) {
    right_array[j] = sequence[middle + 1 + j];
  }

  int k = left;
  for (int i = 0, j = 0; i < size_left && j < size_right; ++k) {
    if (left_array[i] <= right_array[j]) {
      sequence[k] = left_array[i++];
    } else {
      sequence[k] = right_array[j++];
    }
  }

  for (int i = 0; i < size_left; ++i, ++k) {
    sequence[k] = left_array[i];
  }
  for (int j = 0; j < size_right; ++j, ++k) {
    sequence[k] = right_array[j];
  }
}

bool shmelev_stl::ShmelevTaskStl::pre_processing() {
  internal_order_test();
  sequence = *reinterpret_cast<std::vector<int>*>(taskData->inputs[0]);
  result = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
  return true;
}

bool shmelev_stl::ShmelevTaskStl::validation() {
  internal_order_test();
  return taskData->outputs_count[0] == taskData->inputs_count[0];
}

bool shmelev_stl::ShmelevTaskStl::run() {
  internal_order_test();
  batcherMergeSort(0, sequence.size() - 1);
  return true;
}

bool shmelev_stl::ShmelevTaskStl::post_processing() {
  internal_order_test();
  *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]) = sequence;
  return true;
}

void shmelev_stl::ShmelevTaskStl::shellSort() {
  int n = sequence.size();
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      int temp = sequence[i];
      int j = i;
      while (j >= gap && sequence[j - gap] > temp) {
        sequence[j] = sequence[j - gap];
        j -= gap;
      }
      sequence[j] = temp;
    }
  }
}

void shmelev_stl::ShmelevTaskStl::batcherMergeSort(int left, int right) {
  shellSort();
  if (right > left) {
    int middle = left + (right - left) / 2;
    std::thread leftThread(&shmelev_stl::ShmelevTaskStl::batcherMergeSort, this, left, middle);
    std::thread rightThread(&shmelev_stl::ShmelevTaskStl::batcherMergeSort, this, middle + 1, right);

    leftThread.join();
    rightThread.join();
    
    mergeSequences(left, middle, right);
  }
}

void shmelev_stl::ShmelevTaskStl::mergeSequences(int left, int middle, int right) {
  std::vector<int> merged(right - left + 1);
  int i = left;
  int j = middle + 1;
  int k = 0;

  for (; i <= middle && j <= right; ++k) {
    if (sequence[i] <= sequence[j]) {
      merged[k] = sequence[i++];
    } else {
      merged[k] = sequence[j++];
    }
  }

  for (; i <= middle; ++i, ++k) {
    merged[k] = sequence[i];
  }
  for (; j <= right; ++j, ++k) {
    merged[k] = sequence[j];
  }

  for (int idx = 0; idx < k; ++idx) {
    sequence[left + idx] = merged[idx];
  }
}