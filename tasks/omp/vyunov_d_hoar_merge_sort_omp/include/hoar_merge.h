// Copyright 2024 Vyunov Danila
#include <algorithm>
#include <vector>

#include "core/task/include/task.hpp"

class HoareSortSimpleSeq : public ppc::core::Task {
 public:
  explicit HoareSortSimpleSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> array{};
};

void HoarSort(std::vector<int>* arr, int first, int last);
std::vector<int> Getvec(int n);

class HoareSortOMP : public ppc::core::Task {
 public:
  explicit HoareSortOMP(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  static void HoareSortParallel(std::vector<int>& arr, size_t first, size_t last);

 private:
  std::vector<int> array{};
};
