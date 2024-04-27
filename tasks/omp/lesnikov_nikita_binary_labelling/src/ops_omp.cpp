// Copyright 2024 Lesnikov Nikita
#include "omp/lesnikov_nikita_binary_labelling/include/ops_omp.hpp"

#include <omp.h>

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <list>

using namespace std::chrono_literals;

std::vector<int> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100 + 1;
  }
  return vec;
}

template <class T>
T& get(std::vector<T>& v, int n, int x, int y) {
  return v[x * n + y];
}

template <class T>
T get(const std::vector<T>& v, int n, int x, int y) {
  return v[x * n + y];
}

std::vector<uint8_t> serializeInt32(uint32_t num) {
  std::vector<uint8_t> result;
  for (int i = 3; i >= 0; i--) {
    result.push_back(static_cast<uint8_t>(((num << (3 - i) * 8) >> (3 - i) * 8) >> i * 8));
  }
  return result;
}

uint32_t deserializeInt32(const uint8_t* data) {
  uint32_t res = 0;
  for (int i = 3; i >= 0; i--) {
    res += static_cast<uint32_t>(data[3 - i]) << i * 8;
  }
  return res;
}

template <class T>
void visualize(const std::vector<T>& v, int m, int n) {
  std::cout << "\n\n";
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << (int)get(v, n, i, j) << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

std::vector<int> reducePointers(const std::vector<int*> v) {
  std::vector<int> res(v.size());
  for (int i = 0; i < v.size(); i++) {
    res[i] = v[i] ? *v[i] : 0;
  }
  return res;
}

void processHorizontal(std::vector<int*>& labelled, std::list<int>& labels, const std::vector<uint8_t>& v, int& label,
                       int n, int start) {
  for (int j = 1; j < n; j++) {
    if (get(v, n, 0, j)) {
      continue;
    } else if (!get(labelled, n, start, j - 1) && !get(labelled, n, start, j)) {
      labels.push_back(++label);
      get(labelled, n, start, j) = &labels.back();
    } else if (get(labelled, n, start, j - 1) && get(labelled, n, start, j)) {
      *get(labelled, n, start, j) = *get(labelled, n, start, j - 1);
    } else if (get(labelled, n, start, j - 1) && !get(labelled, n, start, j)) {
      get(labelled, n, start, j) = get(labelled, n, start, j - 1);
    }
  }
}

void processVertical(std::vector<int*>& labelled, std::list<int>& labels, const std::vector<uint8_t>& v, int& label,
    int n, int start, int end) {
  for (int i = start + 1; i < end; i++) {
    if (get(v, n, i, 0)) {
      continue;
    } else if (!get(labelled, n, i - 1, 0) && !get(labelled, n, i, 0)) {
      labels.push_back(++label);
      get(labelled, n, i, 0) = &labels.back();
    } else if (get(labelled, n, i - 1, 0)) {
      *get(labelled, n, i, 0) = *get(labelled, n, i - 1, 0);
    }
  }
}

void processLabelled(std::vector<int*>& labelled, std::list<int>& labels, int& label, int n, int i, int j) {
  if (!get(labelled, n, i, j - 1) && get(labelled, n, i - 1, j)) {
    *get(labelled, n, i, j) = *get(labelled, n, i - 1, j);
  } else if (get(labelled, n, i, j - 1) && !get(labelled, n, i - 1, j)) {
    *get(labelled, n, i, j) = *get(labelled, n, i, j - 1);
  } else if (get(labelled, n, i, j - 1) && get(labelled, n, i - 1, j)) {
    *get(labelled, n, i, j) = *get(labelled, n, i - 1, j);
    *get(labelled, n, i, j - 1) = *get(labelled, n, i - 1, j);
  }
}

void processUnlabelled(std::vector<int*>& labelled, std::list<int>& labels, int& label, int n, int i, int j) {
  if (!get(labelled, n, i, j - 1) && !get(labelled, n, i - 1, j)) {
    labels.push_back(++label);
    get(labelled, n, i, j) = &labels.back();
  } else if (!get(labelled, n, i, j - 1) && get(labelled, n, i - 1, j)) {
    get(labelled, n, i, j) = get(labelled, n, i - 1, j);
  } else if (get(labelled, n, i, j - 1) && !get(labelled, n, i - 1, j)) {
    get(labelled, n, i, j) = get(labelled, n, i, j - 1);
  } else if (get(labelled, n, i, j - 1) && get(labelled, n, i - 1, j)) {
    *get(labelled, n, i, j - 1) = *get(labelled, n, i - 1, j);
    get(labelled, n, i, j) = get(labelled, n, i - 1, j);
  }
}

void processPixel(std::vector<int*>& labelled, std::list<int>& labels, int& label, int n, int i, int j) {
  if (get(labelled, n, i, j)) {
    processLabelled(labelled, labels, label, n, i, j);
  } else {
    processUnlabelled(labelled, labels, label, n, i, j);
  }
}

void processMedium(std::vector<int*>& labelled, std::list<int>& labels, const std::vector<uint8_t>& v, int& label,
                   int n, int start, int end) {
  for (int i = start + 1; i < end; i++) {
    for (int j = 1; j < n; j++) {
      if (get(v, n, i, j)) {
        continue;
      }
      processPixel(labelled, labels, label, n, i, j);
    }
  }
}

std::vector<int> getLabelledImageSeq(const std::vector<uint8_t>& v, int m, int n) {
  std::vector<int*> labelled(v.size(), nullptr);
  std::list<int> labels;

  int label = 0;

  if (v.size() && !v[0]) {
    labels.push_back(++label);
    labelled[0] = &labels.back();
  }

  processHorizontal(labelled, labels, v, label, n, 0);
  processVertical(labelled, labels, v, label, n, 0, m);
  processMedium(labelled, labels, v, label, n, 0, m);

  return reducePointers(labelled);
}

std::vector<int> getLabelledImageOmp(const std::vector<uint8_t>& v, int m, int n) {
  std::vector<int*> labelled(v.size(), nullptr);
  std::list<int> labels;
  int label = 0;
  const int numThreads = std::min(8, m / 2);
  const int blockSize = m / numThreads;
  const int dataSizeForThread = (blockSize + 1) * n;

#pragma omp parallel num_threads(numThreads) private(label) private(labels)
  { 
    int tid = omp_get_thread_num();
    label = dataSizeForThread * tid;
    int start = blockSize * tid;
    int end = blockSize * (tid + 1);

  }


}

bool TestOMPTaskSequential::pre_processing() {
  internal_order_test();
  try {
    _source.resize(taskData->inputs_count[0]);
    memcpy(_source.data(), taskData->inputs[0], taskData->inputs_count[0]);
    _m = deserializeInt32(taskData->inputs[1]);
    _n = deserializeInt32(taskData->inputs[2]);
  } catch (...) {
    return false;
  }

  return true;
}

bool TestOMPTaskSequential::validation() {
  internal_order_test();

  return taskData->inputs_count.size() == 3 && taskData->outputs_count.size() == 2 && taskData->inputs_count[1] == 4 &&
         taskData->inputs_count[2] == 4 && taskData->outputs_count[1] == 4 &&
         taskData->inputs_count[0] == deserializeInt32(taskData->inputs[1]) * deserializeInt32(taskData->inputs[2]);
}

bool TestOMPTaskSequential::run() {
  internal_order_test();
  if (ops == "+") {
    res = std::accumulate(input_.begin(), input_.end(), 1);
  } else if (ops == "-") {
    res -= std::accumulate(input_.begin(), input_.end(), 0);
  } else if (ops == "*") {
    res = std::accumulate(input_.begin(), input_.end(), 1, std::multiplies<>());
  }
  std::this_thread::sleep_for(20ms);
  return true;
}

bool TestOMPTaskSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<int*>(taskData->outputs[0])[0] = res;
  return true;
}

bool TestOMPTaskParallel::pre_processing() {
  internal_order_test();
  try {
    _source.resize(taskData->inputs_count[0]);
    memcpy(_source.data(), taskData->inputs[0], taskData->inputs_count[0]);
    _m = deserializeInt32(taskData->inputs[1]);
    _n = deserializeInt32(taskData->inputs[2]);
  } catch (...) {
    return false;
  }

  return true;
}

bool TestOMPTaskParallel::validation() {
  internal_order_test();

  return taskData->inputs_count.size() == 3 && taskData->outputs_count.size() == 2 && taskData->inputs_count[1] == 4 &&
         taskData->inputs_count[2] == 4 && taskData->outputs_count[1] == 4 &&
         taskData->inputs_count[0] == deserializeInt32(taskData->inputs[1]) * deserializeInt32(taskData->inputs[2]);
}

bool TestOMPTaskParallel::run() {
  internal_order_test();
  double start = omp_get_wtime();
  auto temp_res = res;
  if (ops == "+") {
#pragma omp parallel for reduction(+ : temp_res)
    for (int i = 0; i < static_cast<int>(input_.size()); i++) {
      temp_res += input_[i];
    }
  } else if (ops == "-") {
#pragma omp parallel for reduction(- : temp_res)
    for (int i = 0; i < static_cast<int>(input_.size()); i++) {
      temp_res -= input_[i];
    }
  } else if (ops == "*") {
#pragma omp parallel for reduction(* : temp_res)
    for (int i = 0; i < static_cast<int>(input_.size()); i++) {
      temp_res *= input_[i];
    }
  }
  res = temp_res;
  double finish = omp_get_wtime();
  std::cout << "How measure time in OpenMP: " << finish - start << std::endl;
  return true;
}

bool TestOMPTaskParallel::post_processing() {
  internal_order_test();
  reinterpret_cast<int*>(taskData->outputs[0])[0] = res;
  return true;
}