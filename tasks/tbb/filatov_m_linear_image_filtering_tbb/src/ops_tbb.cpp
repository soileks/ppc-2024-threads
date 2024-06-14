// Copyright 2024 Filatov Maxim
#include "tbb/filatov_m_linear_image_filtering_tbb/include/ops_tbb.hpp"

#include <cstdint>

filatov_tbb::Color::Color() { R = G = B = 0; }

filatov_tbb::ColorF::ColorF() { R = G = B = .0f; }

void filatov_tbb::GaussFilterHorizontal::initializeData() {
  input = taskData->inputs[0];
  output = taskData->outputs[0];
  width = taskData->inputs_count[0];
  height = taskData->inputs_count[1];
}

void filatov_tbb::GaussFilterHorizontal::copyInputToImage() {
  image.resize(width * height * 3);
  std::memcpy(image.data(), input, width * height * 3);
}

bool filatov_tbb::GaussFilterHorizontal::pre_processing() {
  internal_order_test();
  initializeData();
  copyInputToImage();
  makeKernel();
  return true;
}

bool filatov_tbb::GaussFilterHorizontal::validation_is_input_size_valid() {
  return taskData->inputs_count[0] >= 3 && taskData->inputs_count[1] >= 3;
}

bool filatov_tbb::GaussFilterHorizontal::validation_is_output_size_valid_first() {
  return taskData->outputs_count[0] == taskData->inputs_count[0];
}

bool filatov_tbb::GaussFilterHorizontal::validation_is_output_size_valid_second() {
  return taskData->outputs_count[1] == taskData->inputs_count[1];
}

bool filatov_tbb::GaussFilterHorizontal::validation() {
  internal_order_test();
  bool isInputSizeValid = validation_is_input_size_valid();
  bool isOutputSizeValidFirst = validation_is_output_size_valid_first();
  bool isOutputSizeValidSecond = validation_is_output_size_valid_second();
  bool isOutputSizeValid = isOutputSizeValidFirst && isOutputSizeValidSecond;
  return isInputSizeValid && isOutputSizeValid;
}

bool filatov_tbb::GaussFilterHorizontal::run() {
  internal_order_test();
  applyKernel();
  return true;
}

bool filatov_tbb::GaussFilterHorizontal::copyImageData() {
  std::memcpy(output, image.data(), width * height * 3);
  return true;
}

bool filatov_tbb::GaussFilterHorizontal::post_processing() {
  internal_order_test();
  return copyImageData();
}

void filatov_tbb::GaussFilterHorizontal::calculateGaussianValues(float sigma, float* normalizationFactor) {
  int64_t halfSize = kSize * .5f;
  for (int64_t row = -halfSize; row <= halfSize; row++) {
    for (int64_t col = -halfSize; col <= halfSize; col++) {
      double gaussianValue = std::exp(-(row * row + col * col) / (2 * sigma * sigma));
      kernel[row + halfSize][col + halfSize] = gaussianValue;
      *normalizationFactor += gaussianValue;
    }
  }
}

void filatov_tbb::GaussFilterHorizontal::normalizeKernel(float normalizationFactor) {
  for (uint32_t row = 0; row < kSize; row++) {
    for (uint32_t col = 0; col < kSize; col++) {
      kernel[row][col] /= normalizationFactor;
    }
  }
}

void filatov_tbb::GaussFilterHorizontal::makeKernel(float sigma) {
  float normalizationFactor = 0;
  calculateGaussianValues(sigma, &normalizationFactor);
  normalizeKernel(normalizationFactor);
}
void filatov_tbb::GaussFilterHorizontal::applyKernel() {
  tbb::parallel_for(tbb::blocked_range2d<uint32_t>(0, height, 0, width), [&](const tbb::blocked_range2d<uint32_t>& r) {
    for (uint32_t i = r.rows().begin(); i != r.rows().end(); ++i) {
      for (uint32_t j = r.cols().begin(); j != r.cols().end(); ++j) {
        size_t index = i * width + j;
        image[index] = calculateNewPixelColor(i, j);
      }
    }
  });
}

void filatov_tbb::GaussFilterHorizontal::calculateSingleColorComponent(uint8_t neighborColor, float kernelValue,
                                                                       float* newColorComponent) {
  *newColorComponent += neighborColor * kernelValue;
}

void filatov_tbb::GaussFilterHorizontal::calculateColorsComponents(Color* neighborColor, int64_t k, int64_t l,
                                                                   int64_t halfSize, ColorF* color) {
  calculateSingleColorComponent(neighborColor->R, kernel[k + halfSize][l + halfSize], &color->R);
  calculateSingleColorComponent(neighborColor->G, kernel[k + halfSize][l + halfSize], &color->G);
  calculateSingleColorComponent(neighborColor->B, kernel[k + halfSize][l + halfSize], &color->B);
}

void filatov_tbb::GaussFilterHorizontal::calculateColorComponentsForRow(int64_t l, size_t x, size_t y, ColorF* color) {
  int64_t halfSize = kSize * .5f;
  for (int64_t k = -halfSize; k <= halfSize; k++) {
    auto idX = clamp<size_t>((x + k), 0, width - 1);
    auto idY = clamp<size_t>((y + l), 0, height - 1);
    Color neighborColor = image[idX * width + idY];
    calculateColorsComponents(&neighborColor, k, l, halfSize, color);
  }
}

void filatov_tbb::GaussFilterHorizontal::calculateColorComponents(size_t x, size_t y, ColorF* color) {
  int64_t halfSize = kSize * .5f;
  for (int64_t l = -halfSize; l <= halfSize; l++) {
    calculateColorComponentsForRow(l, x, y, color);
  }
}

filatov_tbb::Color filatov_tbb::GaussFilterHorizontal::calculateCeilColorF(ColorF preparedColor) {
  Color resultColor;
  resultColor.R = (uint8_t)std::ceil(preparedColor.R);
  resultColor.G = (uint8_t)std::ceil(preparedColor.G);
  resultColor.B = (uint8_t)std::ceil(preparedColor.B);
  return resultColor;
}

filatov_tbb::Color filatov_tbb::GaussFilterHorizontal::calculateNewPixelColor(size_t x, size_t y) {
  ColorF preparedColor;
  calculateColorComponents(x, y, &preparedColor);
  return calculateCeilColorF(preparedColor);
}

template <typename T>
T filatov_tbb::GaussFilterHorizontal::clamp(const T& val, const T& min, const T& max) {
  if (val < min) {
    return min;
  }
  if (val > max) {
    return max;
  }
  return val;
}
