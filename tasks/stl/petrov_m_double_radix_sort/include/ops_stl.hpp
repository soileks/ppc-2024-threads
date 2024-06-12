
#pragma once

#include<random>
#include <thread>
#include <vector>
#include <future>
#include <algorithm>
#include <iterator>
#include "core/task/include/task.hpp"

using namespace std::chrono_literals;
namespace petrov_stl{

class PetrovRadixSortDoubleSTL : public ppc::core::Task {
 public:
  explicit PetrovRadixSortDoubleSTL(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
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
  static std::vector<std::vector<double>> PetrovSplitVector(const std::vector<double>& data, int numParts);
  static std::vector<double> PetrovMerge(const std::vector<double>& arr1, const std::vector<double>& arr2);
  std::vector<double> PetrovRadixSortStl(const std::vector<double>& data, int numParts);
  std::vector<double> PetrovBinaryMergeTree(std::vector<std::vector<double>>& sortedVectors);
};
}  // namespace petrov_stl
