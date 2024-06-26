// Copyright 2024 Pushkarev Ilya

#include "tbb/pushkarev_i_dijkstra_graph_tbb/include/dijkstra_shortest_path.hpp"

#include <algorithm>
#include <memory>
#include <thread>

const int max_int = 2147483647;

bool pushkarev_tbb::DijkstraTaskTBB::pre_processing() {
  internal_order_test();
  graph = *reinterpret_cast<std::vector<std::vector<int>>*>(taskData->inputs[0]);
  source = *reinterpret_cast<int*>(taskData->inputs[1]);
  distances_ = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
  for (size_t i = 0; i < distances_.size(); ++i) {
    distances_[i] = max_int;
  }
  distances_[source] = 0;
  return true;
}

bool pushkarev_tbb::DijkstraTaskTBB::validation() {
  internal_order_test();
  std::vector<std::vector<int>> tmp_graph = *reinterpret_cast<std::vector<std::vector<int>>*>(taskData->inputs[0]);
  for (size_t i = 0; i < tmp_graph.size(); i++) {
    for (size_t j = 0; j < tmp_graph.size(); j++) {
      if (tmp_graph[i][j] < 0) {
        return false;
      }
    }
  }
  return true;
}

bool pushkarev_tbb::DijkstraTaskTBB::run() {
  internal_order_test();

  int n = distances_.size();
  tbb::concurrent_priority_queue<Node> pq;
  pq.emplace(source, 0);

  while (!pq.empty()) {
    Node v;
    if (pq.try_pop(v)) {
      tbb::parallel_for(0, n, [&](int i) {
        if (i == v.vertex) return;

        Node e(i, graph[v.vertex][i]);
        if (e.cost == 0) return;

        int weight = e.cost + v.cost;
        if (weight < distances_[e.vertex]) {
          distances_[e.vertex] = weight;
          pq.emplace(e.vertex, weight);
        }
      });
    }
  }
  return true;
}

void pushkarev_tbb::DijkstraTaskTBB::relaxVertex(size_t u, size_t v) {
  if (distances_[v] > distances_[u] + graph[u][v]) {
    distances_[v] = distances_[u] + graph[u][v];
  }
}

bool pushkarev_tbb::DijkstraTaskTBB::post_processing() {
  internal_order_test();
  *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]) = distances_;
  return true;
}

bool pushkarev_tbb::DijkstraTask::pre_processing() {
  internal_order_test();
  graph = *reinterpret_cast<std::vector<std::vector<int>>*>(taskData->inputs[0]);
  source = *reinterpret_cast<int*>(taskData->inputs[1]);
  distances_ = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
  return true;
}

bool pushkarev_tbb::DijkstraTask::validation() {
  internal_order_test();
  std::vector<std::vector<int>> tmp_graph = *reinterpret_cast<std::vector<std::vector<int>>*>(taskData->inputs[0]);
  for (size_t i = 0; i < tmp_graph.size(); i++) {
    for (size_t j = 0; j < tmp_graph.size(); j++) {
      if (tmp_graph[i][j] < 0) {
        return false;
      }
    }
  }
  return true;
}

bool pushkarev_tbb::DijkstraTask::run() {
  internal_order_test();
  size_t n = distances_.size();
  for (size_t i = 0; i < n; i++) {
    distances_[i] = max_int;
  }
  distances_[source] = 0;
  std::vector<bool> processed(n, false);

  for (size_t count = 0; count < n; count++) {
    size_t u = getMinDistanceVertex(processed);
    processed[u] = true;
    for (size_t v = 0; v < n; v++) {
      if (!processed[v] && (graph[u][v] != 0) && (distances_[u] != max_int) &&
          distances_[u] + graph[u][v] < distances_[v]) {
        relaxVertex(u, v);
      }
    }
  }
  return true;
}

bool pushkarev_tbb::DijkstraTask::post_processing() {
  internal_order_test();

  *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]) = distances_;
  return true;
}

size_t pushkarev_tbb::DijkstraTask::getMinDistanceVertex(const std::vector<bool>& processed) {
  size_t min_dist = max_int;
  size_t min_index = 0;

  for (size_t v = 0; v < distances_.size(); v++) {
    if (!processed[v] && (size_t)distances_[v] <= min_dist) {
      min_dist = distances_[v];
      min_index = v;
    }
  }
  return min_index;
}

void pushkarev_tbb::DijkstraTask::relaxVertex(size_t u, size_t v) {
  if (distances_[v] > distances_[u] + graph[u][v]) {
    distances_[v] = distances_[u] + graph[u][v];
  }
}
