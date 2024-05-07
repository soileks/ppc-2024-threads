//
// Created by Стёпа on 29.03.2024.
//

#include "tbb/kashin_s_dijkstra_algorithm/include/Dijkstra.hpp"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

bool KashinDijkstraTbb::Dijkstra::pre_processing() {
    internal_order_test();
    graph = reinterpret_cast<int*>(taskData->inputs[0]);
    count = taskData->outputs_count[0];
    start = taskData->inputs_count[1];
    distance = std::vector(count, INT_MAX);
    distance[start] = 0;
    return true;
}

bool KashinDijkstraTbb::Dijkstra::validation() {
    internal_order_test();
    int countg = taskData->inputs_count[0];
    int countd = taskData->outputs_count[0];
    int startm = taskData->inputs_count[1];
    if (countg / countd != countd) {
        return false;
    }
    if (startm < 0 || startm >= countd) {
        return false;
    }
    if (countg < 0 || countd < 0) {
        return false;
    }

    return true;
}

bool KashinDijkstraTbb::Dijkstra::run() {
    internal_order_test();
    const int num_threads = tbb::task_scheduler_init::default_num_threads();
    tbb::parallel_for(tbb::blocked_range<int>(0, count, 1),
        [&](const tbb::blocked_range<int>& r) {
            for (int i = r.begin(); i < r.end(); ++i) {
                std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, KashinDijkstraTbb::Compare> tpq;
                // Process graph segments to build initial distance
                while (!tpq.empty()) {
                    std::pair<int, int> vertex = tpq.top();
                    tpq.pop();
                    for (int j = 0; j < count; ++j) {
                        std::pair<int, int> edge(graph[vertex.second * count + j], j);
                        if (j != vertex.second && edge.first != -1) {
                            int weight = edge.first + vertex.first;
                            if (weight < distance[edge.second]) {
                                distance[edge.second] = weight;
                                tpq.emplace(weight, edge.second);
                            }
                        }
                    }
                }
            }
        }
    );

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, KashinDijkstraTbb::Compare> pq;
    pq.emplace(0, start);
    while (!pq.empty()) {
        std::pair<int, int> vertex = pq.top();
        pq.pop();
        for (int i = 0; i < count; i++) {
            std::pair<int, int> edge(graph[vertex.second * count + i], i);
            if (i != vertex.second && edge.first != -1) {
                int weight = edge.first + vertex.first;
                if (weight < distance[edge.second]) {
                    distance[edge.second] = weight;
                    pq.emplace(weight, edge.second);
                }
            }
        }
    }
    return true;
}

bool KashinDijkstraTbb::Dijkstra::post_processing() {
    internal_order_test();
    int* out_ptr = reinterpret_cast<int*>(taskData->outputs[0]);
    tbb::parallel_for(tbb::blocked_range<int>(0, count, 1),
        [&](const tbb::blocked_range<int>& r) {
            for (int i = r.begin(); i < r.end(); ++i) {
                out_ptr[i] = distance[i];
            }
        }
    );
    return true;
}