// Copyright 2024 Salaev Vladislav
#pragma once

#include <algorithm>
#include <mutex>
#include <random>
#include <set>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace SalaevSTL {

class ImageMarkingSeq : public ppc::core::Task {
 public:
  explicit ImageMarkingSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<std::vector<uint8_t>> source = {};
  std::vector<std::vector<uint32_t>> destination = {};
  uint32_t height{}, width{};
  void resolve_labels(std::vector<int>& labels);
};

class ImageMarkingSTL : public ppc::core::Task {
 public:
  explicit ImageMarkingSTL(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<std::vector<uint8_t>> source = {};
  std::vector<std::vector<uint32_t>> destination = {};
  uint32_t height{}, width{};
  static void resolve_labels(std::vector<int>& labels);
};

}  // namespace SalaevSTL