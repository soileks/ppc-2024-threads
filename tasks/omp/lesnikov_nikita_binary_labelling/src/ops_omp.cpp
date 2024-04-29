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
#include <unordered_set>
#include <unordered_map>

using namespace std::chrono_literals;

std::vector<uint8_t> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<uint8_t> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = static_cast<uint8_t>(gen() > std::mt19937::max() / 2);
  }
  return vec;
}

bool isMapsEqual(const std::vector<int>& map1, const std::vector<int>& map2) { 
  if (map1.size() != map2.size()) {
    return false;
  }
  std::unordered_map<int, int> corresp;
  for (size_t i = 0; i < map1.size(); i++) {
    if (!map1[i] && !map2[i])
        continue;
    if ((map1[i] && !map2[i]) || (map2[i] && !map1[i]))
      return false;
    if (corresp.find(map1[i]) == corresp.end())
      corresp[map1[i]] = map2[i];
    else if (corresp[map1[i]] != map2[i])
      return false;
  }
  return true;
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

std::vector<int> deserializeInt32V(const std::vector<uint8_t> v) {
  std::vector<int> res(v.size() / sizeof(int));
  for (size_t i = 0; i < v.size(); i += 4) {
    res[i / 4] = deserializeInt32(v.data() + i);
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

std::vector<int> reducePointers(std::vector<InfPtr>& labelled) {
  std::vector<int> reduced(labelled.size());
  for (int i = 0; i < labelled.size(); i++) {
    reduced[i] = labelled[i].value();
  }
  return reduced;
}

void processHorizontal(std::vector<InfPtr>& labelled, const std::vector<uint8_t>& v, int& label,
                       int n, int start) {
  for (int j = 1; j < n; j++) {
    if (get(v, n, start, j)) {
      continue;
    } else if (!get(labelled, n, start, j - 1).value()) {
      get(labelled, n, start, j).set(new InfPtr(++label));
    } else {
      get(labelled, n, start, j) = get(labelled, n, start, j - 1);
    }
  } 
}

void processVertical(std::vector<InfPtr>& labelled, const std::vector<uint8_t>& v, int& label,
    int n, int start, int end) {
  for (int i = start + 1; i < end; i++) {
    if (get(v, n, i, 0)) {
      continue;
    } else if (!get(labelled, n, i - 1, 0).value()) {
      get(labelled, n, i, 0).set(new InfPtr(++label));
    } else {
      get(labelled, n, i, 0) = get(labelled, n, i - 1, 0);
    }
  }
}

void processUnlabelled(std::vector<InfPtr>& labelled, int& label, int n, int i, int j) {
  if (!get(labelled, n, i, j - 1).value() && !get(labelled, n, i - 1, j).value()) {
    get(labelled, n, i, j).set(new InfPtr(++label));
  } else if (!get(labelled, n, i, j - 1).value() && get(labelled, n, i - 1, j).value()) {
    get(labelled, n, i, j) = get(labelled, n, i - 1, j);
  } else if (get(labelled, n, i, j - 1).value() && !get(labelled, n, i - 1, j).value()) {
    get(labelled, n, i, j) = get(labelled, n, i, j - 1);
  } else if (get(labelled, n, i, j - 1).value() && get(labelled, n, i - 1, j).value()) {
    int value = get(labelled, n, i, j - 1).value();
    InfPtr* ptr = new InfPtr(value);
    get(labelled, n, i, j - 1).set(ptr);
    get(labelled, n, i - 1, j).set(ptr);
    get(labelled, n, i, j).set(ptr);
  }
}

void processMedium(std::vector<InfPtr>& labelled, const std::vector<uint8_t>& v, int& label,
                   int n, int start, int end) {
  for (int i = start + 1; i < end; i++) {
    for (int j = 1; j < n; j++) {
      if (get(v, n, i, j)) {
        continue;
      }
      processUnlabelled(labelled, label, n, i, j);
    }
  }
}

std::pair<std::vector<int>, int> getLabelledImageSeq(const std::vector<uint8_t>& v, int m, int n) {
  std::vector<InfPtr> labelled(v.size());

  int label = 0;

  if (v.size() && !v[0]) {
    labelled[0].set(new InfPtr(++label));
  }

  processHorizontal(labelled, v, label, n, 0);
  processVertical(labelled, v, label, n, 0, m);
  processMedium(labelled, v, label, n, 0, m);

  std::vector<int> reduced = reducePointers(labelled);
  std::unordered_set<int> labelSet(reduced.begin(), reduced.end());

  return std::pair<std::vector<int>, int>(reduced, static_cast<int>(labelSet.size()));
}

void mergeBounds(std::vector<InfPtr>& labelled, int blockSize, int m, int n) { 
  for (int i = blockSize; i < m - 1; i += blockSize) {
    for (int j = 0; j < n; j++) {
      if (get(labelled, n, i - 1, j).value() && get(labelled, n, i, j).value()) {
        int value = get(labelled, n, i, j).value();
        get(labelled, n, i - 1, j).set(new InfPtr(value));
        get(labelled, n, i, j).set(new InfPtr(value));
      }
    }
  }
}

std::pair<std::vector<int>, int> getLabelledImageOmp(const std::vector<uint8_t>& v, int m, int n) {
  std::vector<InfPtr> labelled(v.size());
  const int numThreads = std::min(8, m / 2);
  const int blockSize = m / numThreads;
  const int dataSizeForThread = (blockSize + 1) * n;

#pragma omp parallel num_threads(numThreads)
  { 
    int tid = omp_get_thread_num();
    int label = dataSizeForThread * tid;
    int start = blockSize * tid;
    int end = blockSize * (tid + 1);
    if (tid == numThreads - 1 && m % numThreads != 0) {
      end += m % numThreads;
    }
    if (!get(v, n, start, 0)) {
      get(labelled, n, start, 0).set(new InfPtr(++label));
    }
    processHorizontal(labelled, v, label, n, start);
    processVertical(labelled, v, label, n, start, end);
    processMedium(labelled, v, label, n, start, end);
  }
  mergeBounds(labelled, blockSize, m, n);

  std::vector<int> reduced = reducePointers(labelled);
  std::unordered_set<int> labels(reduced.begin(), reduced.end());

  return std::make_pair(reduced, static_cast<int>(labels.size()));
}

bool BinaryLabellingSeq::pre_processing() {
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

bool BinaryLabellingSeq::validation() {
  internal_order_test();

  return taskData->inputs_count.size() == 3 && taskData->outputs_count.size() == 2 && taskData->inputs_count[1] == 4 &&
         taskData->inputs_count[2] == 4 && taskData->outputs_count[1] == 4 &&
         taskData->inputs_count[0] == deserializeInt32(taskData->inputs[1]) * deserializeInt32(taskData->inputs[2]);
}

bool BinaryLabellingSeq::run() {
  internal_order_test();
  try {
    auto res = getLabelledImageSeq(_source, _m, _n);
    _result = res.first;
    _numObjects = res.second;
  } catch (...) {
    return false;
  }
  return true;
}


bool BinaryLabellingSeq::post_processing() {
  internal_order_test();
  try {
    memcpy(taskData->outputs[0], reinterpret_cast<uint8_t*>(_result.data()),
           _result.size() * static_cast<size_t>(sizeof(int)));
    auto serializedObjectsNum = serializeInt32(_numObjects);
    memcpy(taskData->outputs[1], serializedObjectsNum.data(), serializedObjectsNum.size());
  } catch (...) {
    return false;
  }
  return true;
}

bool BinaryLabellingOmp::pre_processing() {
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

bool BinaryLabellingOmp::validation() {
  internal_order_test();

  return taskData->inputs_count.size() == 3 && taskData->outputs_count.size() == 2 && taskData->inputs_count[1] == 4 &&
         taskData->inputs_count[2] == 4 && taskData->outputs_count[1] == 4 &&
         taskData->inputs_count[0] == deserializeInt32(taskData->inputs[1]) * deserializeInt32(taskData->inputs[2]);
}

bool BinaryLabellingOmp::run() {
  internal_order_test();
  double start = omp_get_wtime();
  try {
    auto res = getLabelledImageOmp(_source, _m, _n);
    _result = res.first;
    _numObjects = res.second;
  } catch (...) {
    return false;
  }
  double finish = omp_get_wtime();
  std::cout << "How measure time in OpenMP: " << finish - start << std::endl;
  return true;
}

bool BinaryLabellingOmp::post_processing() {
  internal_order_test();
  try {
    memcpy(taskData->outputs[0], reinterpret_cast<uint8_t*>(_result.data()),
        _result.size() * static_cast<size_t>(sizeof(int)));
    auto serializedObjectsNum = serializeInt32(_numObjects);
    memcpy(taskData->outputs[1], serializedObjectsNum.data(), serializedObjectsNum.size());
  } catch (...) {
    return false;
  }
  return true;
}
