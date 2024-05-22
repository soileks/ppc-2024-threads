// Copyright 2024 Konovalov Igor
#include "stl/konovalov_i_radix_sort_double/include/ops_stl.hpp"

#include <tbb/tbb.h>

#include <algorithm>
#include <execution>
#include <future>
#include <thread>

using namespace std::chrono_literals;

bool RadixSortSTLTaskParallel::pre_processing() {
  internal_order_test();
  // Init value for input and output
  input_ = reinterpret_cast<double*>(taskData->inputs[0]);
  input_size = taskData->inputs_count[0];
  return true;
}

bool RadixSortSTLTaskParallel::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count.size() == taskData->outputs_count.size();
}

const int max_val = 256;

void RadixSortSTLTaskParallel::radix_sort_seq(double* in, size_t size, std::vector<double>* ordered_local) {
  if (size != 0) {
    char* buf;
    //[sizeof(double) * max_val];
    for (size_t i = 0; i < sizeof(double); i++) {
      for (size_t j = 0; j < size; j++) {
        buf = reinterpret_cast<char*>(&in[j]);
        ordered_local[(uint8_t)buf[i]].push_back(in[j]);
      }

      size_t cur = 0;
      for (size_t j = 0; j < sizeof(double) * 256; j++) {
        for (size_t k = 0; k < ordered_local[j].size(); k++) {
          in[cur] = ordered_local[j][k];
          cur++;
        }
        ordered_local[j].clear();
      }
    }
  }
}

bool RadixSortSTLTaskParallel::run() {
  internal_order_test();

  try {
    auto* chunks = reinterpret_cast<double*>(std::malloc(input_size * max_val * sizeof(double)));
    size_t chunk_sizes[max_val];
    for (size_t i = 0; i < max_val; i++) {
      chunk_sizes[i] = 0;
    }
    for (size_t i = 0; i < input_size; i++) {
      char val = *(reinterpret_cast<char*>(input_ + i) + sizeof(double) - 1);
      chunks[(uint8_t)val * input_size + chunk_sizes[(uint8_t)val]] = input_[i];
      chunk_sizes[(uint8_t)val]++;
    }
    std::vector<size_t> range(max_val);
    for (int i = 0; i < max_val; i++) range[i] = i;
    std::for_each(std::execution::par, range.begin(), range.end(), [=, this](auto&& i) {
      std::vector<double> local_ordered_chunks[sizeof(double) * max_val];
      radix_sort_seq(chunks + i * input_size, chunk_sizes[i], local_ordered_chunks);
    });
    size_t it = 0;
    for (int64_t i = 0; i < max_val; i++) {
      if (chunk_sizes[i] != 0) {
        for (size_t k = 0; k < chunk_sizes[i]; k++) input_[it + k] = chunks[i * input_size + k];
        it += chunk_sizes[i];
      }
    }
    free(chunks);

  } catch (const std::exception& e) {
    std::cerr << "Double Radix sort error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool RadixSortSTLTaskParallel::post_processing() {
  internal_order_test();
  *reinterpret_cast<double**>(taskData->outputs[0]) = input_;
  return true;
}