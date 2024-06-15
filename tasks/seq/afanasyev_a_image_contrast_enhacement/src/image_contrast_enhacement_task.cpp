// Copyright 2024 Afanasyev Aleksey
#include "seq/afanasyev_a_image_contrast_enhacement/include/image_contrast_enhacement_task.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

using namespace AfanasyevAlekseySeq;

std::vector<Pixel> AfanasyevAlekseySeq::generateRandomPixels(std::size_t size) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> distribution(0, UINT8_MAX);
  std::vector<Pixel> pixels(size);

  for (std::size_t i = 0; i < size; i++) {
    pixels.emplace_back(distribution(gen));
  }

  return pixels;
}

bool AfanasyevAlekseySeq::contrastEnhancementSeq(const std::vector<Pixel>& input_pixels,
                                                 std::vector<Pixel>& output_pixels) {
  Pixel min_pixel = *std::min_element(input_pixels.begin(), input_pixels.end());
  Pixel max_pixel = *std::max_element(input_pixels.begin(), input_pixels.end());

  if (min_pixel == max_pixel) {
    return false;
  }

  for (std::size_t i = 0; i < input_pixels.size(); i++) {
    output_pixels[i] = (input_pixels[i] - min_pixel) * 255 / (max_pixel - min_pixel);
  }

  return true;
}

bool ImageContrastEnhacementTask::validation() {
  internal_order_test();

  this->_input_pixels = *reinterpret_cast<std::vector<Pixel>*>(this->taskData->inputs[0]);
  this->_output_pixels = *reinterpret_cast<std::vector<Pixel>*>(this->taskData->outputs[0]);

  if (this->_input_pixels.size() != this->_output_pixels.size()) {
    return false;
  }

  if (this->_input_pixels.empty()) {
    return false;
  }

  return true;
}

bool ImageContrastEnhacementTask::pre_processing() {
  internal_order_test();

  return true;
}

bool ImageContrastEnhacementTask::run() {
  internal_order_test();

  return contrastEnhancementSeq(this->_input_pixels, this->_output_pixels);
}

bool ImageContrastEnhacementTask::post_processing() {
  internal_order_test();

  *reinterpret_cast<std::vector<Pixel>*>(this->taskData->outputs[0]) = this->_output_pixels;

  return true;
}
