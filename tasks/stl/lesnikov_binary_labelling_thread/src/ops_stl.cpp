// Copyright 2024 Lesnikov Nikita
#include "stl/lesnikov_binary_labelling_thread/include/ops_stl.hpp"

#include <cstring>
#include <future>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std::chrono_literals;

class InfPtr {
 public:
  InfPtr() : _ptr(nullptr), _value(0) {}
  InfPtr(int value) : _ptr(nullptr), _value(value) {}
  void set(const std::shared_ptr<InfPtr>& ptr) {
    if (!_ptr) {
      _ptr = std::shared_ptr<InfPtr>(ptr);
      return;
    }
    _ptr->set(ptr);
  }
  int value() {
    if (!_ptr) {
      return _value;
    }
    return _ptr->value();
  }
  bool hasVal() { return static_cast<bool>(value()); }

 private:
  std::shared_ptr<InfPtr> _ptr;
  int _value;
};

std::vector<uint8_t> getRandomVectorForLab(int sz) {
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
    if (!static_cast<bool>(map1[i]) && !static_cast<bool>(map2[i])) {
      continue;
    }
    if ((static_cast<bool>(map1[i]) && !static_cast<bool>(map2[i])) ||
        (static_cast<bool>(map2[i]) && !static_cast<bool>(map1[i]))) {
      return false;
    }
    if (corresp.find(map1[i]) == corresp.end())
      corresp[map1[i]] = map2[i];
    else if (corresp[map1[i]] != map2[i])
      return false;
  }
  return true;
}

size_t getObjectsNum(const std::vector<int>& map) {
  std::unordered_set<int> labels;
  for (int i : map) {
    if (i != 0) {
      labels.insert(i);
    }
  }
  return labels.size();
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

std::vector<int> deserializeInt32V(const std::vector<uint8_t>& v) {
  std::vector<int> res(v.size() / sizeof(int));
  for (size_t i = 0; i < v.size(); i += 4) {
    res[i / 4] = deserializeInt32(v.data() + i);
  }
  return res;
}

std::vector<int> reducePointers(std::vector<InfPtr>& labelled) {
  std::vector<int> reduced(labelled.size());
  for (size_t i = 0; i < labelled.size(); i++) {
    reduced[i] = labelled[i].value();
  }
  return reduced;
}

std::vector<int> reducePointersThread(std::vector<InfPtr>& labelled, int numThreads) {
  std::vector<int> reduced(labelled.size());

  size_t blockSize = labelled.size() / numThreads;
  size_t remainder = labelled.size() % numThreads;

  auto reduce = [&labelled, &reduced](size_t start, size_t end) {
    for (size_t i = start; i < end; i++) {
      reduced[i] = labelled[i].value();
    }
  };

  std::vector<std::future<void>> futures(static_cast<size_t>(numThreads));

  futures[0] = std::async(std::launch::async, reduce, 0, blockSize + remainder);

  for (size_t i = 1; i < static_cast<size_t>(numThreads); i++) {
    futures[i] = 
      std::async(std::launch::async, reduce, remainder + i * blockSize, remainder + (i + 1) * blockSize);
  }

  for (auto& fut : futures) {
    fut.get();
  }

  return reduced;
}

void processHorizontal(std::vector<InfPtr>& labelled, const std::vector<uint8_t>& v, int& label, int n, int start) {
  for (int j = 1; j < n; j++) {
    if (static_cast<bool>(get(v, n, start, j))) {
      continue;
    }
    if (!get(labelled, n, start, j - 1).hasVal()) {
      get(labelled, n, start, j).set(std::make_shared<InfPtr>(++label));
    } else {
      get(labelled, n, start, j) = get(labelled, n, start, j - 1);
    }
  }
}

void processVertical(std::vector<InfPtr>& labelled, const std::vector<uint8_t>& v, int& label, int n, int start,
                     int end) {
  for (int i = start + 1; i < end; i++) {
    if (static_cast<bool>(get(v, n, i, 0))) {
      continue;
    }
    if (!get(labelled, n, i - 1, 0).hasVal()) {
      get(labelled, n, i, 0).set(std::make_shared<InfPtr>(++label));
    } else {
      get(labelled, n, i, 0) = get(labelled, n, i - 1, 0);
    }
  }
}

void processUnlabelled(std::vector<InfPtr>& labelled, int& label, int n, int i, int j) {
  if (!get(labelled, n, i, j - 1).hasVal() && !get(labelled, n, i - 1, j).hasVal()) {
    get(labelled, n, i, j).set(std::make_shared<InfPtr>(++label));
  } else if (!get(labelled, n, i, j - 1).hasVal() && get(labelled, n, i - 1, j).hasVal()) {
    get(labelled, n, i, j) = get(labelled, n, i - 1, j);
  } else if (get(labelled, n, i, j - 1).hasVal() && !get(labelled, n, i - 1, j).hasVal()) {
    get(labelled, n, i, j) = get(labelled, n, i, j - 1);
  } else if (get(labelled, n, i, j - 1).hasVal() && get(labelled, n, i - 1, j).hasVal()) {
    if (get(labelled, n, i, j - 1).value() == get(labelled, n, i - 1, j).value()) {
      get(labelled, n, i, j) = get(labelled, n, i - 1, j);
    } else {
      int value = get(labelled, n, i - 1, j).value();
      auto ptr = std::make_shared<InfPtr>(value);
      get(labelled, n, i, j - 1).set(ptr);
      get(labelled, n, i - 1, j).set(ptr);
      get(labelled, n, i, j) = get(labelled, n, i - 1, j);
    }
  }
}

void processMedium(std::vector<InfPtr>& labelled, const std::vector<uint8_t>& v, int& label, int n, int start,
                   int end) {
  for (int i = start + 1; i < end; i++) {
    for (int j = 1; j < n; j++) {
      if (static_cast<bool>(get(v, n, i, j))) {
        continue;
      }
      processUnlabelled(labelled, label, n, i, j);
    }
  }
}

std::vector<int> getLabelledImageSeq(const std::vector<uint8_t>& v, int m, int n) {
  std::vector<InfPtr> labelled(v.size());

  int label = 0;

  if (!v.empty() && !static_cast<bool>(v[0])) {
    labelled[0].set(std::make_shared<InfPtr>(++label));
  }

  processHorizontal(labelled, v, label, n, 0);
  processVertical(labelled, v, label, n, 0, m);
  processMedium(labelled, v, label, n, 0, m);

  return reducePointers(labelled);
}

void mergeBounds(std::vector<InfPtr>& labelled, int blockSize, int m, int n) {
  for (int i = blockSize; i < m - 1; i += blockSize) {
    for (int j = 0; j < n; j++) {
      if (get(labelled, n, i - 1, j).hasVal() && get(labelled, n, i, j).hasVal() &&
          get(labelled, n, i - 1, j).value() != get(labelled, n, i, j).value()) {
        int value = get(labelled, n, i, j).value();
        auto ptr = std::make_shared<InfPtr>(value);
        get(labelled, n, i - 1, j).set(ptr);
        get(labelled, n, i, j).set(ptr);
      }
    }
  }
}

std::vector<int> getLabelledImageOmp(const std::vector<uint8_t>& v, int m, int n) {
  std::vector<InfPtr> labelled(v.size());
  const int numThreads = std::min(8, m / 2);
  const int blockSize = m / numThreads;
  const int dataSizeForThread = (blockSize + 1) * n;

  auto process = [&](int tid) {
    int label = dataSizeForThread * tid;
    int start = blockSize * tid;
    int end = blockSize * (tid + 1);
    if (tid == numThreads - 1 && m % numThreads != 0) {
      end += m % numThreads;
    }
    if (!static_cast<bool>(get(v, n, start, 0))) {
      get(labelled, n, start, 0).set(std::make_shared<InfPtr>(++label));
    }
    processHorizontal(labelled, v, label, n, start);
    processVertical(labelled, v, label, n, start, end);
    processMedium(labelled, v, label, n, start, end);
  };
  std::vector<std::future<void>> futures(static_cast<size_t>(numThreads));
  for (int i = 0; i < numThreads; i++) {
    futures[i] = std::async(std::launch::async, process, i);
  }
  for (auto& fut : futures) {
    fut.get();
  }
  mergeBounds(labelled, blockSize, m, n);
  return reducePointersThread(labelled, numThreads);
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

  return taskData->inputs_count.size() == 3 && taskData->outputs_count.size() == 1 && taskData->inputs_count[1] == 4 &&
         taskData->inputs_count[2] == 4 &&
         taskData->inputs_count[0] == deserializeInt32(taskData->inputs[1]) * deserializeInt32(taskData->inputs[2]);
}

bool BinaryLabellingSeq::run() {
  internal_order_test();
  try {
    _result = getLabelledImageSeq(_source, _m, _n);
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
  } catch (...) {
    return false;
  }
  return true;
}

bool BinaryLabellingThread::pre_processing() {
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

bool BinaryLabellingThread::validation() {
  internal_order_test();

  return taskData->inputs_count.size() == 3 && taskData->outputs_count.size() == 1 && taskData->inputs_count[1] == 4 &&
         taskData->inputs_count[2] == 4 &&
         taskData->inputs_count[0] == deserializeInt32(taskData->inputs[1]) * deserializeInt32(taskData->inputs[2]);
}

bool BinaryLabellingThread::run() {
  internal_order_test();
  try {
    _result = getLabelledImageOmp(_source, _m, _n);
  } catch (...) {
    return false;
  }
  return true;
}

bool BinaryLabellingThread::post_processing() {
  internal_order_test();
  try {
    memcpy(taskData->outputs[0], reinterpret_cast<uint8_t*>(_result.data()),
           _result.size() * static_cast<size_t>(sizeof(int)));
  } catch (...) {
    return false;
  }
  return true;
}
