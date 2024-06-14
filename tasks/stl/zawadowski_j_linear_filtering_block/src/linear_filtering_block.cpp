// Copyright 2024 Zawadowski Jan

#include "stl/zawadowski_j_linear_filtering_block/include/linear_filtering_block.hpp"

bool zawadaSTL::LinearFiltering::pre_processing() {
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

bool zawadaSTL::LinearFiltering::validation() {
  internal_order_test();
  return taskData->inputs_count[0] >= 3 && taskData->inputs_count[1] >= 3 &&
         taskData->inputs_count[0] == taskData->outputs_count[0] &&
         taskData->inputs_count[1] == taskData->outputs_count[1];
}

bool zawadaSTL::LinearFiltering::run() {
  internal_order_test();

  int numThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(numThreads);

  if (width < blockWidth || height < blockHeight) throw "Error: Image size is less than block size!";

  for (int i = 0; i < numThreads; i++) {
    int startIndex = i * (((height + blockHeight - 1) / blockHeight) / numThreads);
    int endIndex = (i + 1) * (((height + blockHeight - 1) / blockHeight) / numThreads);
    threads[i] = std::thread(&zawadaSTL::LinearFiltering::applyGaussianFilter, this, startIndex, endIndex);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  return true;
}

bool zawadaSTL::LinearFiltering::post_processing() {
  internal_order_test();
  memcpy(output, image.data(), width * height);
  return true;
}

void zawadaSTL::LinearFiltering::sizeSet(uint16_t _width, uint16_t _height) {
  width = _width;
  height = _height;
}

void zawadaSTL::LinearFiltering::blockSet(uint16_t _width, uint16_t _height) {
  blockWidth = _width;
  blockHeight = _height;
}

void zawadaSTL::LinearFiltering::imageSet(std::vector<uint8_t> _image) { image = std::move(_image); }

void zawadaSTL::LinearFiltering::kernelSet(const std::vector<int16_t>& kernel) {
  if (kernel.size() > 9) throw "Error: Kernel larger than 3x3!";
  if (kernel.size() < 9) throw "Error: Kernel smaller than 3x3!";
  gaussKernel = kernel;
}

void zawadaSTL::LinearFiltering::getRandomImage() {
  std::vector<uint8_t> _image(width * height);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, std::numeric_limits<uint8_t>::max());

  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) _image[i * height + j] = dist(gen);

  image = _image;
}

void zawadaSTL::LinearFiltering::applyGaussianFilter(int startIndex, int endIndex) {
  std::vector<uint8_t> resultImage(width * height, 0);

  for (int i = startIndex; i < endIndex; i++)
    for (int j = 0; j < width; j++) {
      float sum = 0.0;
      for (int m = 0; m < 3; m++)
        for (int n = 0; n < 3; n++) {
          int row = i + m;
          int col = j + n;
          if (row >= 0 && row < height && col >= 0 && col < width) {
            int index = row * width + col;
            sum += gaussKernel[m * 3 + n] * image[index];
          }
        }
      resultImage[i * width + j] = sum / std::accumulate(gaussKernel.begin(), gaussKernel.end(), 0);
    }
  image = resultImage;
}
