// Copyright 2024 Kutarin Alexander
#include <gtest/gtest.h>

#include <cstring>
#include <fstream>
#include <random>
#include <vector>
#include <array>

#include "stl/kutarin_a_sobel/include/ops_stl.hpp"

TEST(kutarin_a_sobel_stl, Test_Sobel_Simple_Image_1x1) {
  const int height = 1;
  const int width = 1;

  std::vector<int> inImage = {128};

  std::vector<int> outImage(height * width, 0);

  std::vector<int> resultImage = {0};

  std::shared_ptr<ppc::core::TaskData> taskData_omp = std::make_shared<ppc::core::TaskData>();
  taskData_omp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskData_omp->inputs_count.emplace_back(width);
  taskData_omp->inputs_count.emplace_back(height);
  taskData_omp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskData_omp->outputs_count.emplace_back(width);
  taskData_omp->outputs_count.emplace_back(height);

  KutarinASobel sobelTaskTbb(taskData_omp);
  ASSERT_EQ(sobelTaskTbb.validation(), true);
  sobelTaskTbb.pre_processing();
  sobelTaskTbb.run();
  sobelTaskTbb.post_processing();

  for (int i = 0; i < width * height; ++i) {
    ASSERT_EQ(resultImage[i], outImage[i]);
  }
}

TEST(kutarin_a_sobel_stl, Test_Sobel_Simple_Image_5x5) {
  const int height = 5;
  const int width = 5;

  std::vector<int> inImage = {120, 200, 220, 150, 90, 210, 40,  180, 70,  60,  30,  170, 20,
                              190, 130, 160, 110, 50, 160, 140, 230, 180, 110, 140, 100};

  std::vector<int> outImage(height * width, 0);

  std::vector<int> resultImage = {130, 255, 255, 255, 254, 255, 255, 255, 255, 212, 100, 164, 156,
                                  230, 255, 255, 255, 161, 255, 197, 255, 255, 183, 63,  197};

  std::shared_ptr<ppc::core::TaskData> taskData_omp = std::make_shared<ppc::core::TaskData>();
  taskData_omp->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskData_omp->inputs_count.emplace_back(width);
  taskData_omp->inputs_count.emplace_back(height);
  taskData_omp->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskData_omp->outputs_count.emplace_back(width);
  taskData_omp->outputs_count.emplace_back(height);

  KutarinASobel sobelTaskTbb(taskData_omp);
  ASSERT_EQ(sobelTaskTbb.validation(), true);
  sobelTaskTbb.pre_processing();
  sobelTaskTbb.run();
  sobelTaskTbb.post_processing();

  for (int i = 0; i < width * height; ++i) {
    ASSERT_EQ(resultImage[i], outImage[i]);
  }
}

TEST(kutarin_a_sobel_stl, Test_Sobel_Simple_Image_4x8) {
  constexpr int width = 4;
  constexpr int height = 8;

  std::array<int, width * height> inImage = {102, 88, 47,  121, 169, 26,  158, 201, 159, 178, 145,
                                             107, 82, 112, 54,  5,   47,  198, 27,  181, 229, 174,
                                             51,  71, 172, 92,  57,  244, 75,  125, 130, 140};

  std::array<int, width * height> outImage = {};

  std::array<int, width * height> resultImage = {231, 184, 255, 255, 255, 255, 255, 134, 190, 69,  255,
                                                 255, 255, 210, 255, 105, 255, 255, 255, 255, 255, 255,
                                                 73,  255, 255, 255, 255, 255, 255, 65,  210, 255};

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  KutarinASobel sobelTaskTbb(taskDataTbb);
  ASSERT_EQ(sobelTaskTbb.validation(), true);
  sobelTaskTbb.pre_processing();
  sobelTaskTbb.run();
  sobelTaskTbb.post_processing();

  for (size_t i = 0; i < inImage.size(); ++i) {
    ASSERT_EQ(resultImage[i], outImage[i]);
  }
}

TEST(kutarin_a_sobel_stl, Test_Sobel_Simple_Image_8x8) {
  const int height = 8;
  const int width = 8;

  std::vector<int> inImage = {103, 93,  44,  126, 174, 31,  163, 206, 164, 183, 150, 112, 87,  117, 59,  0,
                              52,  193, 22,  186, 234, 179, 56,  76,  177, 97,  62,  249, 80,  130, 135, 145,
                              1,   7,   149, 103, 93,  44,  126, 174, 31,  163, 206, 164, 183, 150, 112, 87,
                              117, 59,  0,   52,  193, 22,  186, 234, 179, 56,  76,  177, 97,  62,  249, 80};

  std::vector<int> outImage(height * width, 0);

  std::vector<int> resultImage = {
      255, 255, 255, 255, 255, 63,  255, 255, 177, 172, 164, 255, 255, 255, 254, 255, 225, 255, 141, 255, 186, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 117, 255, 255, 255, 255, 140, 255, 130, 228, 213, 255, 255, 255, 176,
      172, 164, 255, 244, 255, 254, 255, 255, 255, 141, 255, 151, 255, 255, 255, 255, 255, 255, 255, 255,
  };

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  KutarinASobel sobelTaskTbb(taskDataTbb);
  ASSERT_EQ(sobelTaskTbb.validation(), true);
  sobelTaskTbb.pre_processing();
  sobelTaskTbb.run();
  sobelTaskTbb.post_processing();

  for (int i = 0; i < width * height; ++i) {
    ASSERT_EQ(resultImage[i], outImage[i]);
  }
}

TEST(kutarin_a_sobel_stl, Test_Sobel_Simple_Image_10x10_Grad) {
  const int height = 10;
  const int width = 10;

  std::vector<int> inImage = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 150, 150, 150, 150, 150, 150,
                              150, 100, 100, 100, 150, 200, 200, 200, 200, 200, 150, 100, 100, 100, 150, 200, 250,
                              250, 250, 200, 150, 100, 100, 100, 150, 200, 250, 255, 250, 200, 150, 100, 100, 100,
                              150, 200, 250, 250, 250, 200, 150, 100, 100, 100, 150, 200, 200, 200, 200, 200, 150,
                              100, 100, 100, 150, 150, 150, 150, 150, 150, 150, 100, 100, 100, 100, 100, 100, 100,
                              100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

  std::vector<int> outImage(height * width, 0);

  std::vector<int> resultImage = {70,  158, 200, 200, 200, 200, 200, 158, 70,  0,   158, 255, 255, 255, 255, 255, 255,
                                  255, 158, 0,   200, 255, 255, 255, 255, 255, 255, 255, 200, 0,   200, 255, 255, 219,
                                  210, 219, 255, 255, 200, 0,   200, 255, 255, 210, 0,   210, 255, 255, 200, 0,   200,
                                  255, 255, 219, 210, 219, 255, 255, 200, 0,   200, 255, 255, 255, 255, 255, 255, 255,
                                  200, 0,   158, 255, 255, 255, 255, 255, 255, 255, 158, 0,   70,  158, 200, 200, 200,
                                  200, 200, 158, 70,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  KutarinASobel sobelTaskTbb(taskDataTbb);
  ASSERT_EQ(sobelTaskTbb.validation(), true);
  sobelTaskTbb.pre_processing();
  sobelTaskTbb.run();
  sobelTaskTbb.post_processing();

  for (int i = 0; i < width * height; ++i) {
    ASSERT_EQ(resultImage[i], outImage[i]);
  }
}

TEST(kutarin_a_sobel_stl, Test_Sobel_SaltAndPepper_Noise_Image) {
  const int height = 15;
  const int width = 15;
  const float noise_ratio = 0.0001f;

  std::vector<int> inImage(height * width, 128);
  std::vector<int> outImage(height * width, 0);
  std::vector<int> resultImage(height * width);

  std::shared_ptr<ppc::core::TaskData> taskDataTbb = std::make_shared<ppc::core::TaskData>();
  taskDataTbb->inputs.emplace_back(reinterpret_cast<uint8_t *>(inImage.data()));
  taskDataTbb->inputs_count.emplace_back(width);
  taskDataTbb->inputs_count.emplace_back(height);
  taskDataTbb->outputs.emplace_back(reinterpret_cast<uint8_t *>(outImage.data()));
  taskDataTbb->outputs_count.emplace_back(width);
  taskDataTbb->outputs_count.emplace_back(height);

  KutarinASobel sobelTaskTbb(taskDataTbb);
  KutarinASobel::generateSaltAndPepperNoise(inImage, height, width, noise_ratio);

  ASSERT_EQ(sobelTaskTbb.validation(), true);
  sobelTaskTbb.pre_processing();
  sobelTaskTbb.run();
  sobelTaskTbb.post_processing();

  // Проверка результатов
  for (int i = 0; i < width * height; ++i) {
    ASSERT_EQ(resultImage[i], outImage[i]);
  }
}
