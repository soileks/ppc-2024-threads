// Copyright 2024 Karagodin Andrey

#include "omp/karagodin_a_dejkstra/include/dejkstra_omp.hpp"

std::vector<std::vector<int>> initGraphMapRandom(int16_t size) {
  if (size < 2) {
    throw std::invalid_argument("Size must be greater than 2");
  }
  std::vector<std::vector<int>> graphMap(size, std::vector<int>(size, 0));
  std::random_device dev;
  std::default_random_engine rng(dev());
  std::uniform_int_distribution<std::default_random_engine::result_type> randNullChance(2, 5);
  std::uniform_int_distribution<std::default_random_engine::result_type> randomization(0, 42);

  #pragma omp parallel for collapse(2)
  for (int16_t i = 0; i < size; i++) {
    for (int16_t j = 0; j < size; j++) {
      if (i != j) {
        if (randNullChance(rng) != 1) {
          graphMap[i][j] = randomization(rng);
          graphMap[j][i] = graphMap[i][j];
        }
      }
    }
  }
  return graphMap;
}

void DejkstraTaskOMP::printGraphMap(const std::vector<std::vector<int>>& graphMapInput) {
  for (const auto& row : graphMapInput) {
    for (int value : row) {
      std::cout << value << "  ";
    }
    std::cout << std::endl;
  }
}

std::pair<std::vector<int>, int> DejkstraTaskOMP::getDejMinPath() {
  size = graphMap.size();
  std::vector<int> dist(size, std::numeric_limits<int>::max());
  dist[entryNode] = 0;

  std::priority_queue<Node, std::vector<Node>, CompareNode> pq;
  pq.push(Node(entryNode, 0));

  while (!pq.empty()) {
    int u = pq.top().vertex;
    pq.pop();

#pragma omp parallel for
    for (int v = 0; v < size; ++v) {
      if (graphMap[u][v] && dist[u] != std::numeric_limits<int>::max() && dist[u] + graphMap[u][v] < dist[v]) {
        dist[v] = dist[u] + graphMap[u][v];
#pragma omp critical
        pq.push(Node(v, dist[v]));
      }
    }
  }
  minScore = dist[destNode];
  pathOutput.clear();
  int currentNode = destNode;
  while (currentNode != entryNode) {
    pathOutput.push_back(currentNode);
    for (int i = 0; i < size; ++i) {
      if (graphMap[i][currentNode] && dist[i] + graphMap[i][currentNode] == dist[currentNode]) {
        currentNode = i;
        break;
      }
    }
  }
  pathOutput.push_back(entryNode);
  std::reverse(pathOutput.begin(), pathOutput.end());
  res = std::make_pair(pathOutput, minScore);
  return res;
}

bool DejkstraTaskOMP::validation() {
  internal_order_test();
  return taskData->inputs[0] != nullptr && taskData->inputs[1] != nullptr && taskData->inputs[2] != nullptr &&
         taskData->inputs_count[0] > 0 && taskData->outputs[0] != nullptr;
}

bool DejkstraTaskOMP::pre_processing() {
  internal_order_test();
  // Init value for input and output
  try {
    entryNode = *reinterpret_cast<int*>(taskData->inputs[0]);
    destNode = *reinterpret_cast<int*>(taskData->inputs[1]);
    graphMap = *reinterpret_cast<std::vector<std::vector<int>>*>(taskData->inputs[2]);
    size = (taskData->inputs_count[0]);
    if (size != 0 && graphMap.data() == nullptr) {
      graphMap = initGraphMapRandom(size);
    }
  } catch (...) {
    return false;
  }
  return true;
}

bool DejkstraTaskOMP::run() {
  try {
    internal_order_test();
    if (size != 0 && graphMap.size() == 0) {
      graphMap = initGraphMapRandom(size);
    }
    res = getDejMinPath();
  } catch (...) {
    return false;
  }
  return true;
}

bool DejkstraTaskOMP::post_processing() {
  internal_order_test();
  auto* result = reinterpret_cast<std::pair<std::vector<int>, int>*>(taskData->outputs[0]);
  result->first.resize(res.first.size());
  result->first = res.first;
  result->second = res.second;
  return true;
}
