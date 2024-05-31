// Copyright 2024 Vanushkin Dmitry
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

using Grayscale = uint8_t;

class Color {
 private:
  uint8_t red, green, blue;

 public:
  explicit Color(Grayscale grayscale) : red(grayscale), green(grayscale), blue(grayscale) {}
  Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}

  Grayscale to_grayscale() const { return (static_cast<uint16_t>(red) + green + blue) / 3; }
};

using ConvolutionKernel = const std::vector<std::vector<int>>;

class SobelOperatorSequential : public ppc::core::Task {
 private:
  size_t imageHeight = {}, imageWidth = {};
  std::vector<Grayscale> grayscaleImage = {};
  std::vector<Grayscale> resultImage = {};

  static ConvolutionKernel convolutionByX;
  static ConvolutionKernel convolutionByY;

 public:
  explicit SobelOperatorSequential(std::shared_ptr<ppc::core::TaskData> taskData) : Task(std::move(taskData)) {}

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  Grayscale apply_convolution(size_t x, size_t y);
};
