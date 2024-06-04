// Copyright 2024 Tushentsova Karina
#include "stl/tushentsova_k_marking_bin_image/include/ops_stl.hpp"

using namespace std::chrono_literals;

bool markingImageStl::run() {
  internal_order_test();
  markingImage();
  return true;
}
bool markingImageStl::validation() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  return (height * width == taskData->inputs_count[1] && taskData->inputs_count[1] == taskData->outputs_count[0]);
}

bool markingImageStl::pre_processing() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  sourse.resize(height);
  destination.resize(height);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j)
      sourse[i].push_back(reinterpret_cast<uint8_t *>(taskData->inputs[1])[i * width + j]);
    destination[i].resize(width, 0);
  }

  return true;
}

bool markingImageStl::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < height; ++i)
    for (size_t j = 0; j < width; ++j)
      reinterpret_cast<uint8_t *>(taskData->outputs[0])[i * width + j] = destination[i][j];
  return true;
}

void markingImageStl::markingImage() {
  uint32_t curLabel = 0;
  std::list<uint32_t> vec;
  std::vector<std::vector<uint32_t *>> arr;
  arr.resize(height);

  int numThreads = std::thread::hardware_concurrency();
  auto *threads = new std::thread[numThreads];
  int rowsPerThread = (height) / numThreads;

  for (int i = 0; i < numThreads; ++i) {
    size_t startRow = i * rowsPerThread;
    size_t endRow = (i == numThreads - 1) ? height : (i + 1) * rowsPerThread;

    threads[i] = std::thread([this, startRow, endRow, &arr] {
      for (size_t i = startRow; i < endRow; ++i) arr[i].resize(width, nullptr);
    });
  }

  for (int p = 0; p < numThreads; ++p) {
    threads[p].join();
  }

  for (size_t i = 0; i < width; ++i) {
    if (sourse[0][i] == 0) {
      if (i == 0 || arr[0][i - 1] == nullptr) {
        vec.push_back(++curLabel);
        arr[0][i] = &vec.back();
      } else
        arr[0][i] = arr[0][i - 1];
    }
  }
  for (size_t i = 1; i < height; ++i) {
    if (sourse[i][0] == 0) {
      if (arr[i - 1][0] == nullptr) {
        vec.push_back(++curLabel);
        arr[i][0] = &vec.back();
      } else
        arr[i][0] = arr[i - 1][0];
    }
    for (size_t j = 1; j < width; ++j) {
      if (sourse[i][j] == 0) {
        if (arr[i - 1][j] == arr[i][j - 1]) {
          if (arr[i - 1][j] == nullptr) {
            vec.push_back(++curLabel);
            arr[i][j] = &vec.back();
          } else
            arr[i][j] = arr[i][j - 1];
        } else {
          if (arr[i - 1][j] == nullptr)
            arr[i][j] = arr[i][j - 1];
          else if (arr[i][j - 1] == nullptr)
            arr[i][j] = arr[i - 1][j];
          else {
            *(arr[i - 1][j]) = *(arr[i][j - 1]);
            arr[i][j] = arr[i][j - 1];
          }
        }
      }
    }
  }

  size_t count = 0;
  size_t cur = 0;
  vec.sort();
  for (auto &label : vec) {
    if (cur != label) {
      cur = label;
      count++;
    }
    label = count;
  }

  for (size_t i = 0; i < height; ++i)
    for (size_t j = 0; j < width; ++j)
      if (arr[i][j] != nullptr) destination[i][j] = *(arr[i][j]);

  delete[] threads;
}