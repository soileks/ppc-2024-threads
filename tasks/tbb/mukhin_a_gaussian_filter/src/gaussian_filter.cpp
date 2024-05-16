// Copyright 2024 Mukhin Ivan
#include "tbb/mukhin_a_gaussian_filter/include/gaussian_filter.hpp"

#include <oneapi/tbb.h>

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "oneapi/tbb/blocked_range.h"
#include "oneapi/tbb/blocked_range2d.h"
#include "oneapi/tbb/parallel_for.h"
#include "oneapi/tbb/task_arena.h"
#include "oneapi/tbb/task_group.h"
#include "tbb/mukhin_a_gaussian_filter/include/pixel_map.hpp"

bool mukhin_i_tbb::GaussianFilterTBB::pre_processing() {
  internal_order_test();
  input = taskData->inputs[0];
  output = taskData->outputs[0];
  width_input = taskData->inputs_count[0];
  height_input = taskData->inputs_count[1];
  width_out = taskData->outputs_count[0];
  height_out = taskData->outputs_count[1];
  image.data.resize(width_input * height_input);
  std::memcpy(image.data.data(), input, width_input * height_input * 3);
  return true;
}

bool mukhin_i_tbb::GaussianFilterTBB::validation() {
  internal_order_test();
  return taskData->inputs_count[0] >= 3 && taskData->inputs_count[1] >= 3 &&
         taskData->outputs_count[0] == taskData->inputs_count[0] &&
         taskData->outputs_count[1] == taskData->inputs_count[1];
}

bool mukhin_i_tbb::GaussianFilterTBB::post_processing() {
  internal_order_test();
  std::memcpy(output, image.data.data(), width_input * height_input * 3);
  return true;
}

bool mukhin_i_tbb::GaussianFilterTBB::run() {
  internal_order_test();
  filter_to_image();
  return true;
}

void mukhin_i_tbb::GaussianFilterTBB::filter_to_image() {
  int GridSize = static_cast<int>(std::sqrt(static_cast<double>(4)));
  int BlockSize = width_input / GridSize;
  tbb::task_arena arena(4);
  arena.execute([&] {
    tbb::parallel_for(tbb::blocked_range<uint32_t>(0, 3), [&](const tbb::blocked_range<uint32_t>& r) {
      for (uint32_t i = r.begin(); i < r.end(); i++) {
        auto thread_id = tbb::this_task_arena::current_thread_index();
        auto i_start = static_cast<uint32_t>((thread_id / GridSize) * BlockSize);
        auto j_start = static_cast<uint32_t>((thread_id % GridSize) * BlockSize);
        for (int i = 0; i < BlockSize; i++) {
          for (int j = 0; j < BlockSize; j++) {
            image.get_pixel(i + i_start, j + j_start) = get_new_pixel(i + i_start, j + j_start);
          }
        }
      }
    });
  });
}

Pixel mukhin_i_tbb::GaussianFilterTBB::get_new_pixel(uint32_t w, uint32_t h) {
  double result_r = 0;
  double result_b = 0;
  double result_g = 0;
  for (int i = -rad; i <= rad; i++) {
    for (int j = -rad; j <= rad; j++) {
      uint32_t new_h = h + j;
      uint32_t new_w = w + i;
      new_h = clamp(new_h, width_input);
      Pixel neighborColor = image.get_pixel(new_w, new_h);
      result_r += neighborColor.r * kernel[i + rad][j + rad];
      result_b += neighborColor.b * kernel[i + rad][j + rad];
      result_g += neighborColor.g * kernel[i + rad][j + rad];
    }
  }
  return Pixel({(uint8_t)std::round(result_r), (uint8_t)std::round(result_g), (uint8_t)std::round(result_b)});
}

uint32_t mukhin_i_tbb::GaussianFilterTBB::clamp(uint32_t value, uint32_t max) {
  if (value < 0) return 0;
  if (value >= max) return max - 1;
  return value;
}

void mukhin_i_tbb::GaussianFilterTBB::create_gaussian_kernel() {
  double sigm = 2.0;
  double norm = 0.0;
  for (int i = -rad; i <= rad; i++) {
    for (int j = -rad; j <= rad; j++) {
      kernel[i + rad][j + rad] = (std::exp(-(i * i + j * j) / (2 * sigm * sigm)));
      norm += kernel[i + rad][j + rad];
    }
  }
  for (uint32_t i = 0; i < kern_size; i++) {
    for (uint32_t j = 0; j < kern_size; j++) {
      kernel[i][j] /= norm;
    }
  }
}
