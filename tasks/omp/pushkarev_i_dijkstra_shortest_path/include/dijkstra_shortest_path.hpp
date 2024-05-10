// Copyright 2024 Pushkarev Ilya
#pragma once

#include <memory>
#include <vector>

#include "core/task/include/task.hpp"

class DijkstraTaskOMP : public ppc::core::Task {
 public:
  explicit DijkstraTaskOMP(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

  struct Node {
    int vertex;
    int cost;

    Node(const int v, const int c) : vertex(v), cost(c) {}
    bool operator>(Node const& n) const { return cost > n.cost; }
    bool operator<(Node const& n) const { return cost < n.cost; }
    friend bool operator<(Node const& n1, Node const& n2) { return n1.cost < n2.cost; }
  };

 private:
  std::vector<int> distances_;

  std::vector<std::vector<int>> graph;
  int source;

  void initializeDistances();
  size_t getMinDistanceVertexOMP(const std::vector<bool>& processed);
  void relaxVertex(size_t u, size_t v);

  // void debug()
  // {
  //   std::cout<<"\n";
  //   for (int i =0;i<(int)distances_.size(); ++i)
  //   {
  //     std::cout<< '['<<i<<"] "<<distances_[i]<<"; ";
  //   }
  //   std::cout<<"\n";
  // }
};

class DijkstraTask : public ppc::core::Task {
 public:
  explicit DijkstraTask(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> distances_;

  std::vector<std::vector<int>> graph;
  int source;

  void initializeDistances();
  size_t getMinDistanceVertex(const std::vector<bool>& processed);
  void relaxVertex(size_t u, size_t v);
};