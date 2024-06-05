// Copyright 2024 Shishkina Valeriya
#include "stl/shishkina_v_gauss_block_stl/include/ops_stl.hpp"

#include <cmath>
#include <functional>
#include <random>
#include <thread>

void LinearFilteringGauss::applyLinearFilteringGauss(int startRow, int endRow) {
  std::vector<int> gaussianKernel = {1, 2, 1, 2, 4, 2, 1, 2, 1};
  int kernelSize = 3;
  for (int i = startRow; i < endRow; ++i) {
      for (int j = 0; j < width; ++j) {
          int sum = 0;
          for (int m = 0; m < kernelSize; ++m) {
              for (int n = 0; n < kernelSize; ++n) {
                  int row = i + m - kernelSize / 2;
                  int col = j + n - kernelSize / 2;
                  if (row >= 0 && row < height && col >= 0 && col < width) {
                      sum += getPixel(row, col) * gaussianKernel[m * kernelSize + n];
                  }
              }
          }
          sum = std::min(sum, 255);
          setPixel(i, j, sum);
      }
  }
}

std::vector<int> getPicture3(int n, int m, uint8_t min, uint8_t max) {
  int size = n * m;
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<int> distrib(min, max);
  std::vector<int> picture(size);
  for (int i = 0; i < size; i++) {
    picture[i] = static_cast<int>(distrib(gen));
  }
  return picture;
}

bool LinearFilteringGauss::pre_processing() {
  internal_order_test();
  height = reinterpret_cast<int *>(taskData->inputs[0])[0];
  width = reinterpret_cast<int *>(taskData->inputs[0])[1];
  min = reinterpret_cast<int *>(taskData->inputs[2])[0];
  max = reinterpret_cast<int *>(taskData->inputs[2])[1];
  for (int i = 0; i < height * width; i++) {
    input.push_back(reinterpret_cast<int *>(taskData->inputs[1])[i]);
    res.push_back(0);
  }
  return true;
}

bool LinearFilteringGauss::validation() {
  internal_order_test();
  bool flag = false;
  if (taskData->inputs_count[1] == taskData->outputs_count[0]) flag = true;
  return flag;
}

bool LinearFilteringGauss::run() {
  internal_order_test();
  std::vector<int> filteredImage(input.size(), 0);
  std::vector<std::thread> threads;
  int blockSize = height / countThreads;

  for (int i = 0; i < countThreads; ++i) {
    int startRow = i * blockSize;
    int endRow;
    if (i == countThreads - 1) {
      endRow = height;
    } else {
      endRow = (i + 1) * blockSize;
    }
    threads.emplace_back([this, startRow, endRow]() { this->applyLinearFilteringGauss(startRow, endRow); });
  }
  for (auto &thread : threads) {
    thread.join();
  }
  threads.clear();
  return true;
}
bool LinearFilteringGauss::post_processing() {
  internal_order_test();
  for (int i = 0; i < height * width; i++) {
    reinterpret_cast<int *>(taskData->outputs[0])[i] = res[i];
  }
  return true;
}