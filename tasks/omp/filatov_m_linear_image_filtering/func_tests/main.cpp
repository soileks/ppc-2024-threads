// Copyright 2024 Filatov Maxim
#include <gtest/gtest.h>

#include <vector>

#include "omp/filatov_m_linear_image_filtering/include/ops_omp.hpp"

using namespace filatov_omp;

class filatov_m_linear_image_filtering_func_omp : public ::testing::Test {
 protected:
  std::shared_ptr<ppc::core::TaskData> taskData;
  std::vector<uint8_t> inputData;
  std::vector<uint8_t> outputData;
  std::vector<uint8_t> expectedData;
  uint64_t width;
  uint64_t height;

  void SetUpWidth(int width_) { this->width = width_; }

  void SetUpHeight(int height_) { this->height = height_; }

  void SetUpWH(int width_, int height_) {
    SetUpWidth(width_);
    SetUpHeight(height_);
  }

  void MySetUp() {
    inputData.resize(width * height * 3);
    outputData.resize(width * height * 3);
    expectedData.resize(width * height * 3);
    taskData = std::make_shared<ppc::core::TaskData>();
  }

  void InitializeTaskData() {
    taskData->inputs.push_back(reinterpret_cast<uint8_t*>(inputData.data()));
    taskData->inputs_count.push_back(width);
    taskData->inputs_count.push_back(height);
    taskData->outputs.push_back(reinterpret_cast<uint8_t*>(outputData.data()));
    taskData->outputs_count.push_back(width);
    taskData->outputs_count.push_back(height);
  }

  void RunFilterAndCheck(bool result) {
    GaussFilterHorizontal gaussFilter(taskData);
    ASSERT_EQ(gaussFilter.validation(), result);
    gaussFilter.pre_processing();
    gaussFilter.run();
    gaussFilter.post_processing();
    ASSERT_EQ(expectedData, outputData);
  }

  void SetInputData(const std::vector<uint8_t>& data) { inputData = data; }

  void SetExpectedData(const std::vector<uint8_t>& data) { expectedData = data; }

  void FillInputData(uint8_t value) { std::fill(inputData.begin(), inputData.end(), value); }
};

TEST_F(filatov_m_linear_image_filtering_func_omp, white_image) {
  SetUpWH(32, 32);
  MySetUp();
  SetInputData(std::vector<uint8_t>(width * height * 3, 255));
  SetExpectedData(std::vector<uint8_t>(width * height * 3, 255));
  InitializeTaskData();
  RunFilterAndCheck(true);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, black_image) {
  SetUpWH(32, 32);
  MySetUp();
  SetInputData(std::vector<uint8_t>(width * height * 3, 0));
  SetExpectedData(std::vector<uint8_t>(width * height * 3, 0));
  InitializeTaskData();
  RunFilterAndCheck(true);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, 0x0_low_resolution_image_false) {
  SetUpWH(0, 0);
  MySetUp();
  SetInputData(std::vector<uint8_t>(width * height * 3, 0));
  SetExpectedData(std::vector<uint8_t>(width * height * 3, 0));
  InitializeTaskData();
  RunFilterAndCheck(false);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, 1x1_low_resolution_image_false) {
  SetUpWH(1, 1);
  MySetUp();
  SetInputData(std::vector<uint8_t>(width * height * 3, 0));
  SetExpectedData(std::vector<uint8_t>(width * height * 3, 0));
  InitializeTaskData();
  RunFilterAndCheck(false);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, 2x2_low_resolution_image_false) {
  SetUpWH(2, 2);
  MySetUp();
  SetInputData(std::vector<uint8_t>(width * height * 3, 0));
  SetExpectedData(std::vector<uint8_t>(width * height * 3, 0));
  InitializeTaskData();
  RunFilterAndCheck(false);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, 3x3_low_resolution_image_true) {
  SetUpWH(3, 3);
  MySetUp();
  std::vector<uint8_t> customInputData = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1};
  std::vector<uint8_t> customExpectedData = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  SetInputData(customInputData);
  SetExpectedData(customExpectedData);
  InitializeTaskData();
  RunFilterAndCheck(true);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, 4x4_low_resolution_image_true) {
  SetUpWH(4, 4);
  MySetUp();
  std::vector<uint8_t> customInputData = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                                          1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1};
  std::vector<uint8_t> customExpectedData = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  SetInputData(customInputData);
  SetExpectedData(customExpectedData);
  InitializeTaskData();
  RunFilterAndCheck(true);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, big_image) {
  SetUpWH(512, 512);
  MySetUp();
  FillInputData(111);
  SetExpectedData(std::vector<uint8_t>(width * height * 3, 111));
  InitializeTaskData();
  RunFilterAndCheck(true);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, red_image) {
  SetUpWH(32, 32);
  MySetUp();
  FillInputData(0);
  for (size_t i = 0; i < inputData.size(); i += 3) {
    inputData[i] = 255;
  }
  SetExpectedData(inputData);
  InitializeTaskData();
  RunFilterAndCheck(true);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, green_image) {
  SetUpWH(32, 32);
  MySetUp();
  FillInputData(0);
  for (size_t i = 1; i < inputData.size(); i += 3) {
    inputData[i] = 255;
  }
  SetExpectedData(inputData);
  InitializeTaskData();
  RunFilterAndCheck(true);
}

TEST_F(filatov_m_linear_image_filtering_func_omp, blue_image) {
  SetUpWH(32, 32);
  MySetUp();
  FillInputData(0);
  for (size_t i = 2; i < inputData.size(); i += 3) {
    inputData[i] = 255;
  }
  SetExpectedData(inputData);
  InitializeTaskData();
  RunFilterAndCheck(true);
}