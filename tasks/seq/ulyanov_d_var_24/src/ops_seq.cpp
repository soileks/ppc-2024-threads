// Copyright 2024 Ulyanov Daniil
#include "seq/ulyanov_d_var_24/include/ops_seq.hpp"

#include <cmath>
#include <thread>

using namespace std::chrono_literals;

void createKernelUlyanov(std::vector<float>& kernel, float sigma) {
  float norm = 0.0;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      kernel[(i + 1) * 3 + (j + 1)] = expf(-static_cast<float>(i * i + j * j) / (2 * sigma * sigma));
      norm += kernel[(i + 1) * 3 + (j + 1)];
    }
  }
  for (int i = 0; i < 9; i++) {
    kernel[i] = kernel[i] / norm;
  }
}

bool FilterGaussHorizontalSequentialUlyanov::pre_processing() {
  internal_order_test();

  height = taskData->inputs_count[0];
  width = taskData->inputs_count[1];

  int* data = reinterpret_cast<int*>(taskData->inputs[0]);

  inputImage = std::vector<Pixel>(height * width);
  resultImage = std::vector<Pixel>(height * width);

  kernel = std::vector<float>(9);
  createKernelUlyanov(kernel, 2.0);

  for (int i = 0; i < height * width * 3; i += 3) {
    inputImage[i / 3].r = static_cast<uint8_t>(data[i]);
    inputImage[i / 3].g = static_cast<uint8_t>(data[i + 1]);
    inputImage[i / 3].b = static_cast<uint8_t>(data[i + 2]);
  }

  return true;
}

bool FilterGaussHorizontalSequentialUlyanov::validation() {
  internal_order_test();

  return taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr && taskData->inputs_count[0] != 0 &&
         taskData->inputs_count[1] != 0 && taskData->outputs_count[0] != 0 && taskData->outputs_count[1] != 0;
}

Pixel calcPixelUlyanov(std::vector<Pixel>& image, int height, int i, int j, std::vector<float>& kernel) {
  Pixel resPixel = Pixel();
  float R = 0.0;
  float G = 0.0;
  float B = 0.0;

  for (int l = -1; l < 2; l++) {
    for (int k = -1; k < 2; k++) {
      R += image[((i + l) * height) + (j + k)].r * kernel[((l + 1) * 3) + (k + 1)];
      G += image[((i + l) * height) + (j + k)].g * kernel[((l + 1) * 3) + (k + 1)];
      B += image[((i + l) * height) + (j + k)].b * kernel[((l + 1) * 3) + (k + 1)];
    }
  }
  resPixel.r = static_cast<uint8_t>(R);
  resPixel.g = static_cast<uint8_t>(G);
  resPixel.b = static_cast<uint8_t>(B);
  return resPixel;
}

bool FilterGaussHorizontalSequentialUlyanov::run() {
  internal_order_test();
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
        resultImage[i * height + j] = inputImage[i * height + j];
      } else {
        resultImage[i * height + j] = calcPixelUlyanov(inputImage, height, i, j, kernel);
      }
    }
  }
  // std::this_thread::sleep_for(20ms);
  return true;
}

bool FilterGaussHorizontalSequentialUlyanov::post_processing() {
  internal_order_test();

  for (int i = 0; i < height * width * 3; i += 3) {
    reinterpret_cast<int*>(taskData->outputs[0])[i] = static_cast<int>(resultImage[i / 3].r);
    reinterpret_cast<int*>(taskData->outputs[0])[i + 1] = static_cast<int>(resultImage[i / 3].g);
    reinterpret_cast<int*>(taskData->outputs[0])[i + 2] = static_cast<int>(resultImage[i / 3].b);
  }

  return true;
}