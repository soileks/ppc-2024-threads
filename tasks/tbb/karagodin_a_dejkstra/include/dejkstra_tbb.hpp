// Copyright 2024 Karagodin Andrey
#pragma once

#ifndef TASKS_TBB_KARAGODIN_A_DEJKSTRA_INCLUDE_DEJKSTRA_TBB_HPP_
#define TASKS_TBB_KARAGODIN_A_DEJKSTRA_INCLUDE_DEJKSTRA_TBB_HPP_

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

class DejkstraTaskTBB : public ppc::core::Task {
public:
  explicit DejkstraTaskTBB(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  std::pair<std::vector<int>, int> getDejMinPath();
  struct Node {
    int vertex;
    int cost;
    Node(int v, int c) : vertex(v), cost(c) {}
  };

  struct CompareNode {
    bool operator()(const Node& n1, const Node& n2) { return n1.cost > n2.cost; }
  };
  static void printGraphMap(const std::vector<std::vector<int>>& graphMapInput);

private:
  int size, destNode, entryNode, minScore;
  std::vector<int> pathOutput;
  std::vector<std::vector<int>> graphMap;
  std::pair<std::vector<int>, int> res;
};

std::vector<std::vector<int>> initGraphMapRandom(int16_t size);

#endif  // TASKS_TBB_KARAGODIN_A_DEJKSTRA_INCLUDE_DEJKSTRA_TBB_HPP_