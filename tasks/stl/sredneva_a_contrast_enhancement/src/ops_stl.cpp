// Copyright 2023 Sredneva Anastasiya
#include "stl/sredneva_a_contrast_enhancement/include/ops_stl.hpp"

#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using namespace std::chrono_literals;

std::vector<uint8_t> getRandomPicture(int n, int m, uint8_t min, uint8_t max) {
  int size = n * m;
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<int> distrib(min, max);
  std::vector<uint8_t> pict(size);
  for (int i = 0; i < size; i++) {
    pict[i] = distrib(gen);
  }
  return pict;
}

bool ContrastEnhancement_STL_Sequential::pre_processing() {
  internal_order_test();
  n = reinterpret_cast<int *>(taskData->inputs[0])[0];
  m = reinterpret_cast<int *>(taskData->inputs[0])[1];
  min = reinterpret_cast<uint8_t *>(taskData->inputs[2])[0];
  max = reinterpret_cast<uint8_t *>(taskData->inputs[2])[1];
  for (int i = 0; i < n * m; i++) {
    input_.push_back(reinterpret_cast<uint8_t *>(taskData->inputs[1])[i]);
    res.push_back(0);
  }
  return true;
}

bool ContrastEnhancement_STL_Sequential::validation() {
  internal_order_test();
  return taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool ContrastEnhancement_STL_Sequential::run() {
  internal_order_test();
  int size = n * m;
  if ((size == 0) || (min == max)) {
    return false;
  }
  for (int i = 0; i < size; i++) {
    res[i] = (input_[i] - min) * 255 / (max - min);
  }
  return true;
}

bool ContrastEnhancement_STL_Sequential::post_processing() {
  internal_order_test();
  for (int i = 0; i < n * m; i++) {
    reinterpret_cast<uint8_t *>(taskData->outputs[0])[i] = res[i];
  }
  return true;
}

bool ContrastEnhancement_STL_Parallel::pre_processing() {
  internal_order_test();
  n = reinterpret_cast<int *>(taskData->inputs[0])[0];
  m = reinterpret_cast<int *>(taskData->inputs[0])[1];
  min = reinterpret_cast<uint8_t *>(taskData->inputs[2])[0];
  max = reinterpret_cast<uint8_t *>(taskData->inputs[2])[1];
  for (int i = 0; i < n * m; i++) {
    input_.push_back(reinterpret_cast<uint8_t *>(taskData->inputs[1])[i]);
    res.push_back(0);
  }
  return true;
}

bool ContrastEnhancement_STL_Parallel::validation() {
  internal_order_test();
  return taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool ContrastEnhancement_STL_Parallel::run() {  // надо изменить
  internal_order_test();
  int size = n * m;
  std::vector<uint8_t> temp_res(size, 0);
  if ((size == 0) || (min == max)) {
    return false;
  }
  const int num_max_threads = std::thread::hardware_concurrency();
  int block = size / num_max_threads;
  std::vector<std::thread> thr(num_max_threads);
  for (int i = 0; i < num_max_threads; i++) {
    int start = i * block;
    int end = (i + 1) * block;
    if (i == num_max_threads - 1) end = size;
    thr[i] = std::thread(
        [&](int start, int end) {
          for (int i = start; i < end; i++) {
            res[i] = ((input_[i] - min) * 255 / (max - min));
          }
        },
        start, end);
  }
  for (int i = 0; i < num_max_threads; i++) {
    thr[i].join();
  }
  return true;
}

bool ContrastEnhancement_STL_Parallel::post_processing() {
  internal_order_test();
  for (int i = 0; i < n * m; i++) {
    reinterpret_cast<uint8_t *>(taskData->outputs[0])[i] = res[i];
  }
  return true;
}

