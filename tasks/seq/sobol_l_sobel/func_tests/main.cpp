// Copyright 2024 Sobol Liubov
#include <gtest/gtest.h>

#include <vector>

#include "seq/sobol_l_sobel/include/sobel_seq.hpp"

TEST(sobol_l_sobel_seq, test_handle_empty_image) {
  int width = 0, height = 0;
  std::vector<sobol::RGB> input(width * height);
  std::vector<sobol::RGB> output(width * height);

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(output.size() * sizeof(sobol::RGB)));

  sobol::Sobel_seq sobel(taskData, width, height);

  ASSERT_FALSE(sobel.validation());
  ASSERT_TRUE(sobel.pre_processing());
  ASSERT_FALSE(sobel.run());
  ASSERT_TRUE(sobel.post_processing());
}

TEST(sobol_l_sobel_seq, test_handle_one_by_one_image) {
  int width = 1, height = 1;

  std::vector<sobol::RGB> input(width * height);
  std::vector<sobol::RGB> output(width * height);

  input = sobol::getRandomRGBPicture(width, height);

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(output.size() * sizeof(sobol::RGB)));

  sobol::Sobel_seq sobel(taskData, width, height);

  ASSERT_TRUE(sobel.validation());
  ASSERT_TRUE(sobel.pre_processing());
  ASSERT_TRUE(sobel.run());
  ASSERT_TRUE(sobel.post_processing());
}

TEST(sobol_l_sobel_seq, test_handle_three_by_three_image) {
  int width = 3, height = 3;

  std::vector<sobol::RGB> input(width * height);
  std::vector<sobol::RGB> output(width * height);

  input = sobol::getRandomRGBPicture(width, height);

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(output.size() * sizeof(sobol::RGB)));

  sobol::Sobel_seq sobel(taskData, width, height);

  ASSERT_TRUE(sobel.validation());
  ASSERT_TRUE(sobel.pre_processing());
  ASSERT_TRUE(sobel.run());
  ASSERT_TRUE(sobel.post_processing());
}

TEST(sobol_l_sobel_seq, test_handle_standard_image) {
  int width = 100, height = 100;

  std::vector<sobol::RGB> input(width * height);
  std::vector<sobol::RGB> output(width * height);

  input = sobol::getRandomRGBPicture(width, height);

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(output.size() * sizeof(sobol::RGB)));

  sobol::Sobel_seq sobel(taskData, width, height);

  ASSERT_TRUE(sobel.validation());
  ASSERT_TRUE(sobel.pre_processing());
  ASSERT_TRUE(sobel.run());
  ASSERT_TRUE(sobel.post_processing());
}

TEST(sobol_l_sobel_seq, test_handle_wide_image) {
  int width = 100, height = 10;

  std::vector<sobol::RGB> input(width * height);
  std::vector<sobol::RGB> output(width * height);

  input = sobol::getRandomRGBPicture(width, height);

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(output.size() * sizeof(sobol::RGB)));

  sobol::Sobel_seq sobel(taskData, width, height);

  ASSERT_TRUE(sobel.validation());
  ASSERT_TRUE(sobel.pre_processing());
  ASSERT_TRUE(sobel.run());
  ASSERT_TRUE(sobel.post_processing());
}

TEST(sobol_l_sobel_seq, test_handle_gradient_image) {
  int width = 10, height = 10;

  std::vector<sobol::RGB> input(width * height);
  std::vector<sobol::RGB> expected_output(width * height);

  input = {{0, 0, 0},       {28, 28, 28},    {56, 56, 56},    {85, 85, 85},    {113, 113, 113}, {141, 141, 141},
           {170, 170, 170}, {198, 198, 198}, {226, 226, 226}, {255, 255, 255}, {0, 0, 0},       {28, 28, 28},
           {56, 56, 56},    {85, 85, 85},    {113, 113, 113}, {141, 141, 141}, {170, 170, 170}, {198, 198, 198},
           {226, 226, 226}, {255, 255, 255}, {0, 0, 0},       {28, 28, 28},    {56, 56, 56},    {85, 85, 85},
           {113, 113, 113}, {141, 141, 141}, {170, 170, 170}, {198, 198, 198}, {226, 226, 226}, {255, 255, 255},
           {0, 0, 0},       {28, 28, 28},    {56, 56, 56},    {85, 85, 85},    {113, 113, 113}, {141, 141, 141},
           {170, 170, 170}, {198, 198, 198}, {226, 226, 226}, {255, 255, 255}, {0, 0, 0},       {28, 28, 28},
           {56, 56, 56},    {85, 85, 85},    {113, 113, 113}, {141, 141, 141}, {170, 170, 170}, {198, 198, 198},
           {226, 226, 226}, {255, 255, 255}, {0, 0, 0},       {28, 28, 28},    {56, 56, 56},    {85, 85, 85},
           {113, 113, 113}, {141, 141, 141}, {170, 170, 170}, {198, 198, 198}, {226, 226, 226}, {255, 255, 255},
           {0, 0, 0},       {28, 28, 28},    {56, 56, 56},    {85, 85, 85},    {113, 113, 113}, {141, 141, 141},
           {170, 170, 170}, {198, 198, 198}, {226, 226, 226}, {255, 255, 255}, {0, 0, 0},       {28, 28, 28},
           {56, 56, 56},    {85, 85, 85},    {113, 113, 113}, {141, 141, 141}, {170, 170, 170}, {198, 198, 198},
           {226, 226, 226}, {255, 255, 255}, {0, 0, 0},       {28, 28, 28},    {56, 56, 56},    {85, 85, 85},
           {113, 113, 113}, {141, 141, 141}, {170, 170, 170}, {198, 198, 198}, {226, 226, 226}, {255, 255, 255},
           {0, 0, 0},       {28, 28, 28},    {56, 56, 56},    {85, 85, 85},    {113, 113, 113}, {141, 141, 141},
           {170, 170, 170}, {198, 198, 198}, {226, 226, 226}, {255, 255, 255}};
  expected_output = {
      {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},
      {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {224, 224, 224},
      {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224},
      {228, 228, 228}, {0, 0, 0},       {0, 0, 0},       {224, 224, 224}, {228, 228, 228}, {228, 228, 228},
      {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {0, 0, 0},
      {0, 0, 0},       {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228},
      {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {0, 0, 0},       {0, 0, 0},       {224, 224, 224},
      {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224},
      {228, 228, 228}, {0, 0, 0},       {0, 0, 0},       {224, 224, 224}, {228, 228, 228}, {228, 228, 228},
      {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {0, 0, 0},
      {0, 0, 0},       {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228},
      {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {0, 0, 0},       {0, 0, 0},       {224, 224, 224},
      {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224},
      {228, 228, 228}, {0, 0, 0},       {0, 0, 0},       {224, 224, 224}, {228, 228, 228}, {228, 228, 228},
      {224, 224, 224}, {228, 228, 228}, {228, 228, 228}, {224, 224, 224}, {228, 228, 228}, {0, 0, 0},
      {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},
      {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0}};

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(expected_output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(expected_output.size() * sizeof(sobol::RGB)));

  sobol::Sobel_seq sobel(taskData, width, height);

  ASSERT_TRUE(sobel.validation());
  ASSERT_TRUE(sobel.pre_processing());
  ASSERT_TRUE(sobel.run());
  ASSERT_TRUE(sobel.post_processing());
  for (size_t i = 0; i < expected_output.size(); ++i) {
    sobol::RGB& out_pixel = *reinterpret_cast<sobol::RGB*>(taskData->outputs[0] + i * sizeof(sobol::RGB));
    sobol::RGB& expected_pixel = expected_output[i];
    ASSERT_EQ(out_pixel.r, expected_pixel.r);
    ASSERT_EQ(out_pixel.g, expected_pixel.g);
    ASSERT_EQ(out_pixel.b, expected_pixel.b);
  }
}
