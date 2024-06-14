// Copyright 2024 Makhinya Danil
#pragma once

#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "core/task/include/task.hpp"

class HoareSortSTL : public ppc::core::Task {
 public:
  using sortable_type = uint32_t;
  using compare_t = std::function<bool(const sortable_type&, const sortable_type&)>;
  using vec_t = std::vector<sortable_type>;

  explicit HoareSortSTL(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

  bool check_order();

 private:
  static bool default_compare(const sortable_type& a, const sortable_type& b) { return a < b; }
  void seq_hoare_sort(sortable_type* first_ptr, sortable_type* last_ptr);
  void parallel_hoare_sort(sortable_type* first_ptr, sortable_type const* last_ptr);
  void sort_bitonic_seguence(sortable_type* first_ptr, sortable_type const* last_ptr);

  vec_t* _data;
  compare_t _comp;
};
