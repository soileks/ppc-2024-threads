// Copyright 2024 Afanasyev Aleksey
#include <gtest/gtest.h>

#include <vector>

#include "stl/afanasyev_a_image_contrast_enhacement/include/image_contrast_enhacement_task.hpp"

using namespace AfanasyevAlekseyStl;

TEST(afanasyev_a_image_contrast_enhacement_stl, validation_error_input_pixels_empty) {
  std::vector<Pixel> input_pixels;
  std::vector<Pixel> output_pixels;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  ImageContrastEnhacementTask task(taskData);
  ASSERT_FALSE(task.validation());
}

TEST(afanasyev_a_image_contrast_enhacement_stl, validation_error_input_output_pixels_size_different) {
  std::vector<Pixel> input_pixels = {1, 2, 3, 4};
  std::vector<Pixel> output_pixels(input_pixels.size() + 10);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  ImageContrastEnhacementTask task(taskData);
  ASSERT_FALSE(task.validation());
}

TEST(afanasyev_a_image_contrast_enhacement_stl, test_small_pixels) {
  std::vector<Pixel> input_pixels = {170, 166, 66, 159, 19, 177};
  std::vector<Pixel> output_pixels(input_pixels.size());
  std::vector<Pixel> expected_output_pixels = {243, 237, 75, 225, 0, 255};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  ImageContrastEnhacementTask task(taskData);
  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  for (std::size_t i = 0; i < output_pixels.size(); i++) {
    ASSERT_EQ(output_pixels[i], expected_output_pixels[i]);
  }
}

TEST(afanasyev_a_image_contrast_enhacement_stl, test_medium_pixels) {
  // clang-format off
  std::vector<Pixel> input_pixels = {117, 235, 120, 220, 19, 225, 117, 112, 22, 112, 192, 67, 162, 73, 139, 133};
  std::vector<Pixel> output_pixels(input_pixels.size());
  std::vector<Pixel> expected_output_pixels = {115, 255, 119, 237, 0, 243, 115, 109, 3, 109, 204, 56, 168, 63, 141, 134};
  // clang-format on

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  ImageContrastEnhacementTask task(taskData);
  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  for (std::size_t i = 0; i < output_pixels.size(); i++) {
    ASSERT_EQ(output_pixels[i], expected_output_pixels[i]);
  }
}

TEST(afanasyev_a_image_contrast_enhacement_stl, test_large_pixels) {
  // clang-format off
  std::vector<Pixel> input_pixels = {248, 91, 235, 170, 175, 16, 243, 130, 0, 94, 140, 152, 62, 240, 152, 51, 134, 248, 128, 170, 87, 230, 81, 72, 9, 18, 240, 75, 114, 130};
  std::vector<Pixel> output_pixels(input_pixels.size());
  std::vector<Pixel> expected_output_pixels = {255, 93, 241, 174, 179, 16, 249, 133, 0, 96, 143, 156, 63, 246, 156, 52, 137, 255, 131, 174, 89, 236, 83, 74, 9, 18, 246, 77, 117, 133};
  // clang-format on

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  ImageContrastEnhacementTask task(taskData);
  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  for (std::size_t i = 0; i < output_pixels.size(); i++) {
    ASSERT_EQ(output_pixels[i], expected_output_pixels[i]);
  }
}

TEST(afanasyev_a_image_contrast_enhacement_stl, test_large_random_pixels) {
  std::vector<Pixel> input_pixels = generateRandomPixels(100);
  std::vector<Pixel> output_pixels(input_pixels.size());
  std::vector<Pixel> expected_output_pixels(input_pixels.size());
  bool ok = contrastEnhancementSeq(input_pixels, expected_output_pixels);
  ASSERT_TRUE(ok);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();

  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(&input_pixels));
  taskData->inputs_count.emplace_back(1);

  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(&output_pixels));
  taskData->outputs_count.emplace_back(1);

  // Create Task
  ImageContrastEnhacementTask task(taskData);
  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  for (std::size_t i = 0; i < output_pixels.size(); i++) {
    ASSERT_EQ(output_pixels[i], expected_output_pixels[i]);
  }
}
