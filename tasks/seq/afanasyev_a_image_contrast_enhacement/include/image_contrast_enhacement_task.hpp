// Copyright 2024 Afanasyev Aleksey
#pragma once

#include <vector>

#include "core/task/include/task.hpp"

namespace AfanasyevAlekseySeq {

using Pixel = std::uint8_t;

std::vector<Pixel> generateRandomPixels(std::size_t size);
bool contrastEnhancementSeq(const std::vector<Pixel>& input_pixels, std::vector<Pixel>& output_pixels);

class ImageContrastEnhacementTask : public ppc::core::Task {
 public:
  explicit ImageContrastEnhacementTask(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool validation() override;
  bool pre_processing() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<Pixel> _input_pixels;
  std::vector<Pixel> _output_pixels;
};

}  // namespace AfanasyevAlekseySeq
