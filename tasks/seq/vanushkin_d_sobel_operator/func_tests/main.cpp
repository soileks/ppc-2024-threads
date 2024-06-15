// Copyright 2024 Vanushkin Dmitry
#include <gtest/gtest.h>

#include <vector>

#include "seq/vanushkin_d_sobel_operator/include/sobel_operator_seq.hpp"

TEST(Vanuskin_D_SeqSobelOperator, InvalidZeroImageSize) {
  std::vector<Color> in;
  std::vector<Grayscale> out;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(0);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));

  SobelOperatorSequential task(taskDataSeq);
  ASSERT_FALSE(task.validation());
}

TEST(Vanuskin_D_SeqSobelOperator, InvalidImageSizes) {
  std::vector<Color> in;
  std::vector<Grayscale> out;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(5);
  taskDataSeq->inputs_count.emplace_back(10);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(5);
  taskDataSeq->outputs_count.emplace_back(10);

  SobelOperatorSequential task(taskDataSeq);
  ASSERT_FALSE(task.validation());
}

TEST(Vanuskin_D_SeqSobelOperator, ValidImageSizes) {
  std::vector<Color> in = {{1, 2, 3}};
  std::vector<Grayscale> out(5, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(800);
  taskDataSeq->inputs_count.emplace_back(600);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(798);
  taskDataSeq->outputs_count.emplace_back(598);

  SobelOperatorSequential task(taskDataSeq);
  ASSERT_TRUE(task.validation());
}

TEST(Vanuskin_D_SeqSobelOperator, InvalidInputOrOutputData) {
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(nullptr);
  taskDataSeq->inputs_count.emplace_back(800);
  taskDataSeq->inputs_count.emplace_back(600);
  taskDataSeq->outputs.emplace_back(nullptr);
  taskDataSeq->outputs_count.emplace_back(798);
  taskDataSeq->outputs_count.emplace_back(598);

  SobelOperatorSequential task(taskDataSeq);
  ASSERT_FALSE(task.validation());
}

TEST(Vanuskin_D_SeqSobelOperator, ApplySobelTo3x3GrayscaleImage) {
  std::vector<Color> input = {
      Color(100), Color(200), Color(60), Color(81), Color(21), Color(240), Color(180), Color(65), Color(96),
  };

  std::vector<Grayscale> output(1, 0);

  Grayscale expectSingle = 247;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(3);
  taskDataSeq->inputs_count.emplace_back(3);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(1);
  taskDataSeq->outputs_count.emplace_back(1);

  SobelOperatorSequential task(taskDataSeq);

  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  ASSERT_EQ(output[0], expectSingle);
}

TEST(Vanuskin_D_SeqSobelOperator, ApplySobelTo3x3FailedGrayscaleImage) {
  std::vector<Color> input = {Color(175), Color(74),  Color(153), Color(180), Color(65),
                              Color(66),  Color(138), Color(177), Color(119)};

  std::vector<Grayscale> output(1, 0);

  Grayscale expectSingle = 255;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(3);
  taskDataSeq->inputs_count.emplace_back(3);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(1);
  taskDataSeq->outputs_count.emplace_back(1);

  SobelOperatorSequential task(taskDataSeq);

  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  ASSERT_EQ(output[0], expectSingle);
}

TEST(Vanuskin_D_SeqSobelOperator, ApplySobelTo3x3ColorizedImage) {
  std::vector<Color> input = {{98, 53, 0},     {4, 200, 201},   {155, 155, 154}, {100, 183, 189}, {61, 148, 26},
                              {237, 100, 226}, {242, 147, 165}, {140, 214, 91},  {247, 117, 182}};

  std::vector<Grayscale> output(1, 0);

  Grayscale expectSingle = 248;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(3);
  taskDataSeq->inputs_count.emplace_back(3);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(1);
  taskDataSeq->outputs_count.emplace_back(1);

  SobelOperatorSequential task(taskDataSeq);

  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  ASSERT_EQ(output[0], expectSingle);
}

TEST(Vanuskin_D_SeqSobelOperator, ApplySobelTo4x7ColorizedImage) {
  size_t height = 7;
  size_t width = 4;

  std::vector<Color> input = {
      {170, 225, 131}, {3, 157, 63},    {120, 222, 117}, {0, 112, 205},   {100, 48, 163}, {13, 217, 54},
      {14, 76, 206},   {227, 163, 152}, {132, 14, 51},   {43, 27, 130},   {37, 255, 53},  {217, 124, 48},
      {211, 239, 189}, {184, 167, 148}, {153, 58, 204},  {229, 208, 95},  {163, 76, 120}, {43, 127, 141},
      {50, 6, 85},     {126, 109, 56},  {89, 34, 169},   {174, 120, 110}, {129, 197, 76}, {212, 242, 221},
      {103, 248, 44},  {111, 229, 195}, {203, 229, 63},  {227, 147, 111},
  };

  std::vector<Grayscale> output((height - 2) * (width - 2), 0);

  std::vector<Grayscale> expected = {
      255, 65, 94, 210, 129, 255, 255, 255, 136, 130,
  };

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(width - 2);
  taskDataSeq->outputs_count.emplace_back(height - 2);

  SobelOperatorSequential task(taskDataSeq);

  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  for (size_t i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(expected[i], output[i]);
  }
}

TEST(Vanuskin_D_SeqSobelOperator, ApplySobelTo9x5ColorizedImage) {
  size_t height = 9;
  size_t width = 5;

  std::vector<Color> input = {
      {57, 29, 174},   {23, 18, 192},   {37, 180, 167},  {137, 42, 217},  {86, 127, 208},  {240, 111, 226},
      {22, 217, 33},   {65, 41, 66},    {222, 143, 72},  {175, 104, 131}, {24, 83, 82},    {234, 110, 120},
      {207, 95, 99},   {78, 109, 224},  {49, 193, 149},  {76, 71, 94},    {213, 174, 236}, {91, 68, 93},
      {125, 226, 165}, {183, 77, 180},  {21, 141, 160},  {211, 155, 13},  {174, 60, 215},  {249, 49, 164},
      {138, 135, 113}, {234, 164, 91},  {166, 2, 41},    {234, 138, 82},  {13, 66, 93},    {167, 245, 161},
      {26, 199, 246},  {237, 126, 46},  {228, 219, 125}, {156, 168, 96},  {218, 221, 97},  {28, 107, 129},
      {38, 159, 83},   {118, 206, 34},  {235, 177, 22},  {50, 7, 140},    {49, 101, 230},  {165, 190, 55},
      {50, 244, 184},  {105, 245, 201}, {42, 122, 182},
  };

  std::vector<Grayscale> output((height - 2) * (width - 2), 0);

  std::vector<Grayscale> expected = {
      208, 174, 167, 149, 198, 162, 120, 62, 110, 147, 119, 132, 122, 78, 78, 103, 66, 41, 142, 154, 164,
  };

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(height - 2);
  taskDataSeq->outputs_count.emplace_back(width - 2);

  SobelOperatorSequential task(taskDataSeq);

  ASSERT_TRUE(task.validation());
  ASSERT_TRUE(task.pre_processing());
  ASSERT_TRUE(task.run());
  ASSERT_TRUE(task.post_processing());

  for (size_t i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(expected[i], output[i]);
  }
}
