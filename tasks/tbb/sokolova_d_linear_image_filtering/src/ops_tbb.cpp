// Copyright 2024 Sokolova Daria
#include "tbb/sokolova_d_linear_image_filtering/include/ops_tbb.hpp"

#include <cstdint>

bool sokolova_tbb::GaussFilterHorizontalTbb::pre_processing() {
  internal_order_test();
  input = taskData->inputs[0];
  output = taskData->outputs[0];
  width = taskData->inputs_count[0];
  height = taskData->inputs_count[1];
  image.resize(width * height * 3);
  std::memcpy(image.data(), input, width * height * 3);
  makeKernel();
  return true;
}

bool sokolova_tbb::GaussFilterHorizontalTbb::validation() {
  internal_order_test();
  return taskData->inputs_count[0] >= 3 && taskData->inputs_count[1] >= 3 &&
         taskData->outputs_count[0] == taskData->inputs_count[0] &&
         taskData->outputs_count[1] == taskData->inputs_count[1];
}

bool sokolova_tbb::GaussFilterHorizontalTbb::run() {
  internal_order_test();
  applyKernel();
  return true;
}

bool sokolova_tbb::GaussFilterHorizontalTbb::post_processing() {
  internal_order_test();
  std::memcpy(output, image.data(), width * height * 3);
  return true;
}

void sokolova_tbb::GaussFilterHorizontalTbb::makeKernel(float sigma) {
  uint32_t radius = kSize / 2;
  uint32_t size = kSize;
  float cm = 0;
  for (int i = -static_cast<int>(radius); i <= static_cast<int>(radius); i++) {
    for (int j = -static_cast<int>(radius); j <= static_cast<int>(radius); j++) {
      double weight = std::exp(-(i * i + j * j) / (2 * sigma * sigma));
      kernel[i + radius][j + radius] = weight;
      cm += weight;
    }
  }
  for (uint32_t i = 0; i < size; i++) {
    for (uint32_t j = 0; j < size; j++) {
      kernel[i][j] /= cm;
    }
  }
}

void sokolova_tbb::GaussFilterHorizontalTbb::applyKernel() {
  tbb::parallel_for(0, static_cast<int>(width), [&](int i) {
    for (int j = 0; j < static_cast<int>(height); j++) {
      auto ii = static_cast<uint32_t>(i);
      auto jj = static_cast<uint32_t>(j);
      image[ii * width + jj] = calculateNewPixelColor(ii, jj);
    }
  });
}

sokolova_tbb::Color sokolova_tbb::GaussFilterHorizontalTbb::calculateNewPixelColor(size_t x, size_t y) {
  sokolova_tbb::Color resultColor;
  uint32_t radius = kSize / 2;
  float newR = 0;
  float newG = 0;
  float newB = 0;
  for (int l = -static_cast<int>(radius); l <= static_cast<int>(radius); l++) {
    for (int k = -static_cast<int>(radius); k <= static_cast<int>(radius); k++) {
      size_t idX = x + k;
      size_t idY = y + l;
      if (idX < 0) idX = 0;
      if (idY < 0) idY = 0;
      if (idX >= width) idX = width - 1;
      if (idY >= height) idY = height - 1;
      Color neighborColor = image[idX * width + idY];
      newR += neighborColor.R * kernel[k + radius][l + radius];
      newG += neighborColor.G * kernel[k + radius][l + radius];
      newB += neighborColor.B * kernel[k + radius][l + radius];
    }
  }
  resultColor.R = static_cast<uint8_t>(std::ceil(newR));
  resultColor.G = static_cast<uint8_t>(std::ceil(newG));
  resultColor.B = static_cast<uint8_t>(std::ceil(newB));
  return resultColor;
}
