// Copyright 2024 Ulyanov Daniil
#pragma once

#include <tbb/tbb.h>

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class TBBFilterGaussHorizontalSeqUlyanov : public ppc::core::Task {
 public:
  explicit TBBFilterGaussHorizontalSeqUlyanov(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> inputImage;
  std::vector<uint8_t> resultImage;
  std::vector<float> kernel;
  int width, height;
};

class TBBFilterGaussHorizontalParUlyanov : public ppc::core::Task {
 public:
  explicit TBBFilterGaussHorizontalParUlyanov(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> inputImage;
  std::vector<uint8_t> resultImage;
  std::vector<float> kernel;
  int width, height;
};