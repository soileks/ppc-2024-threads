// Copyright 2024 Ivanchenko Aleksei
#include "stl/ivanchenko_a_gauss_filter_vertical/include/gauss_stl.hpp"

#include <thread>

using namespace std::chrono_literals;

bool GaussFilterSequential::pre_processing() {
  internal_order_test();
  // Init value for input and output
  input = taskData->inputs[0];
  output = taskData->outputs[0];
  width = taskData->inputs_count[0];
  height = taskData->inputs_count[1];
  image.resize(width * height * 3);
  std::memcpy(image.data(), input, width * height * 3);
  createKernel();
  return true;
}

bool GaussFilterSequential::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] >= 3 && taskData->inputs_count[1] >= 3 &&
         taskData->outputs_count[0] == taskData->inputs_count[0] &&
         taskData->outputs_count[1] == taskData->inputs_count[1];
}

bool GaussFilterSequential::run() {
  internal_order_test();
  applyKernel();
  return true;
}

bool GaussFilterSequential::post_processing() {
  internal_order_test();
  std::memcpy(output, image.data(), width * height * 3);
  return true;
}
void GaussFilterSequential::createKernel(float sigma) {
  uint32_t radius = kernelSize / 2;
  uint32_t size = kernelSize;
  float norm = 0;  // коэффициент нормировки ядра
  for (int i = -(int)radius; i <= (int)radius; i++) {
    for (int j = -(int)radius; j <= (int)radius; j++) {
      kernel[i + radius][j + radius] = (double)(std::exp(-(i * i + j * j) / (2 * sigma * sigma)));
      norm += kernel[i + radius][j + radius];
    }
  }
  // нормируем ядро
  for (uint32_t i = 0; i < size; i++) {
    for (uint32_t j = 0; j < size; j++) {
      kernel[i][j] /= norm;
    }
  }
}
void GaussFilterSequential::applyKernel() {
  uint32_t numThreads = std::thread::hardware_concurrency();
  auto* threads = new std::thread[numThreads];
  uint32_t columnsPerThread = (width - 2) / numThreads;

  for (uint32_t i = 0; i < numThreads; ++i) {
    uint32_t startColumn = i * columnsPerThread + 1;
    uint32_t endColumn = (i == numThreads - 1) ? width - 1 : (i + 1) * columnsPerThread + 1;
    threads[i] = std::thread([this, startColumn, endColumn]() {
      for (uint32_t i = startColumn; i < endColumn; ++i) {
        for (uint32_t j = 0; j < height; ++j) {
          image[i * width + j] = calculateNewPixelColor(i, j);
        }
      }
    });
  }
  for (uint32_t i = 0; i < numThreads; ++i) {
    threads[i].join();
  }

  delete[] threads;
}
Color GaussFilterSequential::calculateNewPixelColor(size_t x, size_t y) {
  uint32_t radius = kernelSize / 2;
  float resultR = 0;
  float resultG = 0;
  float resultB = 0;
  for (int l = -((int)radius); l <= (int)radius; l++) {
    for (int k = -((int)radius); k <= (int)radius; k++) {
      size_t idX = x + k;
      size_t idY = y + l;
      if (idX < 0) idX = 0;
      if (idX >= width) idX = width - 1;
      if (idY < 0) idY = 0;
      if (idY >= height) idY = height - 1;
      Color neighborColor = image[idX * width + idY];
      resultR += neighborColor.R * kernel[k + radius][l + radius];
      resultG += neighborColor.G * kernel[k + radius][l + radius];
      resultB += neighborColor.B * kernel[k + radius][l + radius];
    }
  }
  Color result;
  result.R = (uint8_t)std::ceil(resultR);
  result.G = (uint8_t)std::ceil(resultG);
  result.B = (uint8_t)std::ceil(resultB);
  return result;
}