// Copyright 2024 Zawadowski Jan

#include "tbb/zawadowski_j_linear_filtering_block/include/linear_filtering_block.hpp"

bool zawadaTBB::LinearFiltering::pre_processing() {
  internal_order_test();
  input = taskData->inputs[0];
  output = taskData->outputs[0];
  width = taskData->inputs_count[0];
  height = taskData->inputs_count[1];
  widthOut = taskData->outputs_count[0];
  heightOut = taskData->outputs_count[1];
  image.resize(width * height);
  memcpy(image.data(), input, width * height);
  return true;
}

bool zawadaTBB::LinearFiltering::validation() {
  internal_order_test();
  return taskData->inputs_count[0] >= 3 && taskData->inputs_count[1] >= 3 &&
         taskData->inputs_count[0] == taskData->outputs_count[0] &&
         taskData->inputs_count[1] == taskData->outputs_count[1];
}

bool zawadaTBB::LinearFiltering::run() {
  internal_order_test();
  applyGaussianFilter();
  return true;
}

bool zawadaTBB::LinearFiltering::post_processing() {
  internal_order_test();
  memcpy(output, image.data(), width * height);
  return true;
}

void zawadaTBB::LinearFiltering::sizeSet(uint16_t _width, uint16_t _height) {
  width = _width;
  height = _height;
}

void zawadaTBB::LinearFiltering::blockSet(uint16_t _width, uint16_t _height) {
  blockWidth = _width;
  blockHeight = _height;
}

void zawadaTBB::LinearFiltering::imageSet(std::vector<uint8_t> _image) { image = std::move(_image); }

void zawadaTBB::LinearFiltering::kernelSet(const std::vector<int16_t>& kernel) {
  if (kernel.size() > 9) throw "Error: Kernel larger than 3x3!";
  if (kernel.size() < 9) throw "Error: Kernel smaller than 3x3!";
  gaussKernel = kernel;
}

void zawadaTBB::LinearFiltering::getRandomImage() {
  std::vector<uint8_t> _image(width * height);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, UCHAR_MAX);

  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) _image[i * height + j] = dist(gen);

  image = _image;
}

void zawadaTBB::LinearFiltering::applyGaussianFilter() {
  if (width < blockWidth || height < blockHeight) throw "Error: Image size is less than block size!";

  std::vector<uint8_t> resultImage(width * height, 0);

  tbb::parallel_for(0, static_cast<int>(height), static_cast<int>(blockHeight), [&](const int i) {
    for (int j = 0; j < width; j += blockWidth)
      for (int k = i; k < i + blockHeight; k++)
        for (int l = j; l < j + blockWidth; l++) {
          float sum = 0.0;
          for (int m = 0; m < 3; m++)
            for (int n = 0; n < 3; n++) {
              int row = k + m;
              int col = l + n;
              if (row >= 0 && row < height && col >= 0 && col < width) {
                int index = row * width + col;
                sum += gaussKernel[m * 3 + n] * image[index];
              }
            }
          resultImage[k * width + l] = sum / std::accumulate(gaussKernel.begin(), gaussKernel.end(), 0);
        }
  });
  image = resultImage;
}
