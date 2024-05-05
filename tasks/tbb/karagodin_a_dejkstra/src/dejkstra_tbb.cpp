// Copyright 2024 Karagodin Andrey
#include "tbb/karagodin_a_dejkstra/include/dejkstra_tbb.hpp"

std::vector<std::vector<int>> initGraphMapRandom(int16_t size) {
  if (size < 2) {
    throw std::invalid_argument("Size must be greater than 2");
  }
  std::vector<std::vector<int>> graphMap;
  graphMap.clear();
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> randNullChance(2, 5);
  std::uniform_int_distribution<std::mt19937::result_type> randomization(0, 42);
  int cost = 0;
  graphMap.resize(size);
  for (auto& innerVec : graphMap) {
    innerVec.resize(size);
  }
  tbb::parallel_for(tbb::blocked_range<int16_t>(0, size), [&](const tbb::blocked_range<int16_t>& r) {
    for (int16_t i = r.begin(); i < r.end(); ++i) {
      for (int16_t j = 0; j < size; ++j) {
        if (randNullChance(rng) != 1) {
          cost = randomization(rng);
          graphMap[i][j] = cost;
          graphMap[j][i] = cost;
        } else {
          graphMap[i][j] = 0;
          graphMap[j][i] = 0;
        }
        if (i == j) graphMap[i][j] = 0;
      }
    }
  });

  return graphMap;
}

void DejkstraTaskTBB::printGraphMap(const std::vector<std::vector<int>>& graphMapInput) {
  for (const auto& row : graphMapInput) {
    for (int value : row) {
      std::cout << value << "  ";
    }
    std::cout << std::endl;
  }
}

std::pair<std::vector<int>, int> DejkstraTaskTBB::getDejMinPath() {
  std::vector<int> dist(size, std::numeric_limits<int>::max());
  std::vector<int> prev(size, -1);
  std::priority_queue<Node, std::vector<Node>, CompareNode> pq;
  dist[entryNode] = 0;
  pq.emplace(entryNode, 0);

  // Parallelize the initialization of dist and prev vectors
  tbb::parallel_for(tbb::blocked_range<int16_t>(0, size), [&](const tbb::blocked_range<int16_t>& r) {
    for (int16_t i = r.begin(); i < r.end(); ++i) {
      dist[i] = std::numeric_limits<int>::max();
      prev[i] = -1;
    }
  });

  // The rest of the Dijkstra algorithm remains sequential
  while (!pq.empty()) {
    Node currentnode = pq.top();
    pq.pop();

    int u = currentnode.vertex;

    if (u == destNode) {
      minScore = dist[u];
      break;
    }

    for (int v = 0; v < size; ++v) {
      if (graphMap[u][v] != 0) {
        int alt = dist[u] + graphMap[u][v];
        if (alt < dist[v]) {
          dist[v] = alt;
          prev[v] = u;
          pq.emplace(v, alt);
        }
      }
    }
  }

  // Reconstruct path
  int current = destNode;
  while (current != -1) {
    pathOutput.push_back(current);
    current = prev[current];
  }
  std::reverse(pathOutput.begin(), pathOutput.end());
  res.first = pathOutput;
  res.second = minScore;
  return res;
}

bool DejkstraTaskTBB::validation() {
  internal_order_test();
  return taskData->inputs[0] != nullptr && taskData->inputs[1] != nullptr && taskData->inputs[2] != nullptr &&
         taskData->inputs_count[0] > 0 && taskData->outputs[0] != nullptr;
}

bool DejkstraTaskTBB::pre_processing() {
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

bool DejkstraTaskTBB::run() {
  try {
    internal_order_test();
    res = getDejMinPath();
  } catch (...) {
    return false;
  }
  return true;
}

bool DejkstraTaskTBB::post_processing() {
  internal_order_test();
  auto* result = reinterpret_cast<std::pair<std::vector<int>, int>*>(taskData->outputs[0]);
  result->first.resize(res.first.size());
  result->first = res.first;
  result->second = res.second;
  return true;
}
