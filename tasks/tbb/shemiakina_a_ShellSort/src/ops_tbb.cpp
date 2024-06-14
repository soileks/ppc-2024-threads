// Copyright 2024 Shemiakina Alesya
#include "tbb/shemiakina_a_ShellSort/include/ops_tbb.hpp"

#include <oneapi/tbb/parallel_for.h>

#include <thread>

using namespace std::chrono_literals;

bool ShellTaskTBB::pre_processing() {
  internal_order_test();
  // Init value for input and output
  for (int i = 0; i < static_cast<int>(taskData->inputs_count[0]); i++) {
    input_.push_back(reinterpret_cast<int*>(taskData->inputs[0])[i]);
  }
  return true;
}

bool ShellTaskTBB::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count.size() == 1 && taskData->inputs_count[0] > 0 && taskData->outputs_count.size() == 1 &&
         taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool ShellTaskTBB::run() {
  internal_order_test();
  ShellSort_Parallel(input_);
  return true;
}

bool ShellTaskTBB::post_processing() {
  internal_order_test();
  for (int i = 0; i < static_cast<int>(taskData->inputs_count[0]); i++) {
    reinterpret_cast<int*>(taskData->outputs[0])[i] = input_[i];
  }
  return true;
}

std::vector<int> ShellTaskTBB::merge(const std::vector<std::vector<int>>& chunks) {
  std::vector<int> res;

  // Merge the sorted chunks
  for (const auto& chunk : chunks) {
    // Merge the current chunk with the result vector
    std::vector<int> merged;
    merged.reserve(res.size() + chunk.size());
    auto resIter = res.begin();
    auto chunkIter = chunk.begin();

    while (resIter != res.end() && chunkIter != chunk.end()) {
      if (*resIter < *chunkIter) {
        merged.push_back(*resIter);
        ++resIter;
      } else {
        merged.push_back(*chunkIter);
        ++chunkIter;
      }
    }

    // Copy remaining elements from result vector
    while (resIter != res.end()) {
      merged.push_back(*resIter);
      ++resIter;
    }

    // Copy remaining elements from current chunk
    while (chunkIter != chunk.end()) {
      merged.push_back(*chunkIter);
      ++chunkIter;
    }

    // Update result vector with merged vector
    res = std::move(merged);
  }
  return res;
}

void ShellTaskTBB::ShellSort_Parallel(std::vector<int>& input) {
  int num_procs = 4;
  int size = static_cast<int>(input.size());
  int chunk_size = size / num_procs;
  int remainder = size % num_procs;

  std::vector<std::vector<int>> chunks(num_procs);

  tbb::parallel_for(0, num_procs, [&](int i) {
    int startIdx = i * chunk_size;
    int endIdx = startIdx + chunk_size;
    if (i == num_procs - 1) {
      endIdx += remainder;  // Add remaining elements to the last chunk
    }
    chunks[i].assign(input.begin() + startIdx, input.begin() + endIdx);
    ShellSort(chunks[i]);
  });

  input = merge(chunks);
}

void ShellTaskTBB::ShellSort(std::vector<int>& vec) {
  for (int interval = static_cast<int>(vec.size()) / 2; interval > 0; interval /= 2) {
    for (int i = interval; i < static_cast<int>(vec.size()); i++) {
      int tmp = vec[i];
      int j = i;
      for (; j >= interval && vec[j - interval] > tmp; j = j - interval) {
        vec[j] = vec[j - interval];
      }
      vec[j] = tmp;
    }
  }
}

bool ShellTaskTBB::CheckSort(std::vector<int> input) { return std::is_sorted(input.begin(), input.end()); }

std::vector<int> ShellTaskTBB::give_random_vector(int size, int min, int max) {
  std::random_device rnd_device;
  std::mt19937 mersenne_engine{rnd_device()};
  std::uniform_int_distribution<int> dist{min, max};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

  std::vector<int> vec(size);
  generate(begin(vec), end(vec), gen);

  return vec;
}
