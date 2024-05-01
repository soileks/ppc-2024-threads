// Copyright 2024 Sobol Liubov
#include "stl/sobol_l_sobel/include/sobel_stl.hpp"

#include <cmath>
#include <cstring>
#include <mutex>
#include <random>
#include <thread>

bool sobol::Sobel_stl::validation() {
  internal_order_test();
  if (taskData->inputs.empty() || taskData->outputs.empty()) {
    return false;
  }
  if (taskData->inputs_count.empty() || taskData->outputs_count.empty()) {
    return false;
  }
  if (taskData->inputs_count[0] == 0 || taskData->outputs_count[0] == 0) {
    return false;
  }
  return 3 * width * height * sizeof(uint8_t) == taskData->inputs_count[0];
}

bool sobol::Sobel_stl::pre_processing() {
  internal_order_test();
  input_.resize(width * height);
  res.resize(width * height);
  auto* inputPixels = reinterpret_cast<sobol::RGB*>(taskData->inputs[0]);
  for (int i = 0; i < width * height; i++) {
    input_[i] = inputPixels[i];
  }
  return true;
}

void sobol::Sobel_stl::sobel_thread(int start, int end) {
  for (int i = start; i < end; i++) {
    for (int j = 1; j < height - 1; j++) {
      if (i > 0 && i < width - 1) {
        process_pixel(i, j);
      }
    }
  }
}

bool sobol::Sobel_stl::run() {
  internal_order_test();
  int size = width * height;
  if (size == 0) {
    return false;
  }
  if (size == 1) {
    res[0] = input_[0];
    return true;
  }

  int rows_per_thread = width / num_threads;
  threads.reserve(num_threads);
  for (int i = 0; i < num_threads; i++) {
    int start = i * rows_per_thread;
    int end = (i == num_threads - 1) ? width : (i + 1) * rows_per_thread;
    threads.emplace_back(&sobol::Sobel_stl::sobel_thread, this, start, end);
  }
  for (auto& thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  return true;
}

bool sobol::Sobel_stl::post_processing() {
  internal_order_test();
  for (int i = 0; i < width * height; i++) {
    memcpy(taskData->outputs[0] + i * sizeof(sobol::RGB), &res[i], sizeof(sobol::RGB));
  }
  return true;
}

void sobol::Sobel_stl::process_pixel(int i, int j) {
  const int xKernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  const int yKernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  double gx = 0;
  double gy = 0;
  double gray = 0;

  for (int di = -1; di <= 1; ++di) {
    for (int dj = -1; dj <= 1; ++dj) {
      int ni = i + di;
      int nj = j + dj;
      int idx_n = ni * height + nj;

      sobol::RGB& pixel = input_[idx_n];
      gray = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;

      gx += gray * xKernel[di + 1][dj + 1];
      gy += gray * yKernel[di + 1][dj + 1];
    }
  }

  int grad = static_cast<int>(sqrt(gx * gx + gy * gy));
  grad = std::min(255, std::max(0, grad));

  res[i * height + j].r = res[i * height + j].g = res[i * height + j].b = grad;
}

std::vector<sobol::RGB> sobol::getRandomRGBPicture(int w, int h) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);
  std::vector<sobol::RGB> picture(w * h);

  for (int i = 0; i < w * h; i++) {
    picture[i] = {static_cast<uint8_t>(dis(gen)), static_cast<uint8_t>(dis(gen)), static_cast<uint8_t>(dis(gen))};
  }
  return picture;
}