// Copyright 2024 Mukhin Ivan
#pragma once
#include <sys/types.h>

#include <cstdint>

#include "core/task/include/task.hpp"
#include "seq/mukhin_a_gaussian_filter/include/pixel_map.hpp"

class GaussianFilterSeq : public ppc::core::Task {
 public:
  explicit GaussianFilterSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  static const uint64_t kern_size = 3;
  int rad = kern_size / 2;
  double kernel[kern_size][kern_size] = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
  uint8_t *input, *output;
  uint64_t width_input, height_input, width_out, height_out;
  PixelMap image;

  void create_gaussian_kernel();

  static uint64_t clamp(uint64_t value, uint64_t max);

  void filter_to_image();

  Pixel get_new_pixel(uint64_t w, uint64_t h);
};
