// Copyright 2024 Kruglov Alexey
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace KruglovOmpTask{

std::vector<uint8_t> getRandomVector(int sz);
bool isMapsEqual(const std::vector<int>& map1, const std::vector<int>& map2);
size_t getObjectsNum(const std::vector<int>& map);
std::vector<uint8_t> serializeInt32(uint32_t num);
uint32_t deserializeInt32(const uint8_t* data);
std::vector<int> deserializeInt32V(const std::vector<uint8_t>& v);

class imgMarkingSeq : public ppc::core::Task {
 public:
  explicit imgMarkingSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> source;
  std::vector<int> result;
  uint32_t _m = 0;
  uint32_t _n = 0;
};

class imgMarkingOmp : public ppc::core::Task {
 public:
  explicit imgMarkingOmp(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> source;
  std::vector<int> result;
  uint32_t _m = 0;
  uint32_t _n = 0;
};
}