// Copyright 2024 Petrov Maksim

#pragma once
#include <random>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class PetrovRadixSortDoubleSequential : public ppc::core::Task {
 public:
  explicit PetrovRadixSortDoubleSequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  int data_size;
  std::vector<double> sort;
  static void PetrovCountSort(double* in, double* out, int len, int exp);
  static bool PetrovCountSortSigns(const double* in, double* out, int len);
  static std::vector<double> PetrovRadixSort(const std::vector<double>& data1);
};