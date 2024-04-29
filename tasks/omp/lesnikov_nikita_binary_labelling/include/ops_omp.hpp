// Copyright 2024 Lesnikov Nikita
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

std::vector<uint8_t> getRandomVector(int sz);
bool isMapsEqual(const std::vector<int>& map1, const std::vector<int>& map2);
std::vector<uint8_t> serializeInt32(uint32_t num);
uint32_t deserializeInt32(const uint8_t* data);
std::vector<int> deserializeInt32V(const std::vector<uint8_t> v);

class InfPtr {
 public:
  InfPtr() = default;
  InfPtr(int value) : _value(value), _ptr(nullptr) {}
  void set(InfPtr* ptr) {
    if (!_ptr) {
      _ptr = ptr;
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
 private:
  InfPtr* _ptr = nullptr;
  int _value = 0;
};

class BinaryLabellingSeq : public ppc::core::Task {
 public:
  explicit BinaryLabellingSeq(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> _source;
  std::vector<int> _result;
  uint32_t _m = 0;
  uint32_t _n = 0;
  uint32_t _numObjects = 0;
};

class BinaryLabellingOmp : public ppc::core::Task {
 public:
  explicit BinaryLabellingOmp(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> _source;
  std::vector<int> _result;
  uint32_t _m = 0;
  uint32_t _n = 0;
  uint32_t _numObjects = 0;
};
