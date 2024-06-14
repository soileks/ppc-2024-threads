// Copyright 2024 Skotin Alexander
#include <gtest/gtest.h>

#include <cstring>
#include <memory>
#include <vector>

#include "tbb/skotin_a_multiply_matrix_cannon/include/ops_tbb.hpp"

namespace SkotinTBBFuncTests {

std::vector<uint8_t> createMatrixData(size_t size, double value) {
  std::vector<double> matrixData(size * size, value);
  std::vector<uint8_t> byteData(matrixData.size() * sizeof(double));
  memcpy(byteData.data(), matrixData.data(), byteData.size());
  return byteData;
}

void checkMatrixMultiplicationResult(const std::vector<uint8_t>& outputData, size_t matrixSize, double expectedValue) {
  for (size_t i = 0; i < matrixSize; ++i) {
    for (size_t j = 0; j < matrixSize; ++j) {
      double actualValue;
      memcpy(&actualValue, &outputData[(i * matrixSize + j) * sizeof(double)], sizeof(double));
      EXPECT_NEAR(actualValue, expectedValue, 1e-8) << "Mismatch at (" << i << "," << j << ")";
    }
  }
}

void TestMatrixMultiplication(size_t matrixSize, double fillValueA, double fillValueB, double expectedValue) {
  auto matrixAData = createMatrixData(matrixSize, fillValueA);
  auto matrixBData = createMatrixData(matrixSize, fillValueB);

  auto taskSeqData = std::make_shared<ppc::core::TaskData>();
  taskSeqData->inputs.push_back(matrixAData.data());
  taskSeqData->inputs_count.push_back(matrixAData.size());
  taskSeqData->inputs.push_back(matrixBData.data());
  taskSeqData->inputs_count.push_back(matrixBData.size());

  std::vector<uint8_t> outputData1(matrixSize * matrixSize * sizeof(double));
  taskSeqData->outputs.push_back(outputData1.data());
  taskSeqData->outputs_count.push_back(outputData1.size());

  SkotinMatrixMultiplicationTBBSeq taskSeq(taskSeqData);
  ASSERT_TRUE(taskSeq.pre_processing());
  ASSERT_TRUE(taskSeq.validation());
  ASSERT_TRUE(taskSeq.run());
  ASSERT_TRUE(taskSeq.post_processing());

  checkMatrixMultiplicationResult(outputData1, matrixSize, expectedValue);

  auto taskParralelData = std::make_shared<ppc::core::TaskData>();
  taskParralelData->inputs.push_back(matrixAData.data());
  taskParralelData->inputs_count.push_back(matrixAData.size());
  taskParralelData->inputs.push_back(matrixBData.data());
  taskParralelData->inputs_count.push_back(matrixBData.size());

  std::vector<uint8_t> outputData2(matrixSize * matrixSize * sizeof(double));
  taskParralelData->outputs.push_back(outputData2.data());
  taskParralelData->outputs_count.push_back(outputData2.size());

  SkotinMatrixMultiplicationTBBParallel taskParralel(taskParralelData);
  ASSERT_TRUE(taskParralel.pre_processing());
  ASSERT_TRUE(taskParralel.validation());
  ASSERT_TRUE(taskParralel.run());
  ASSERT_TRUE(taskParralel.post_processing());

  checkMatrixMultiplicationResult(outputData2, matrixSize, expectedValue);
}

}  // namespace SkotinTBBFuncTests

TEST(Skotin_A_Multiply_Matrix_Cannon_Seq, Test_Multiplication_5x5) {
  SkotinTBBFuncTests::TestMatrixMultiplication(5, 1.0, 2.0, 10.0);
}

TEST(Skotin_A_Multiply_Matrix_Cannon_Seq, Test_Multiplication_10x10) {
  SkotinTBBFuncTests::TestMatrixMultiplication(10, 1.0, 2.0, 20.0);
}

TEST(Skotin_A_Multiply_Matrix_Cannon_Seq, Test_Multiplication_30x30) {
  SkotinTBBFuncTests::TestMatrixMultiplication(30, 1.0, 2.0, 60.0);
}

TEST(Skotin_A_Multiply_Matrix_Cannon_Seq, Test_Multiplication_100x100) {
  SkotinTBBFuncTests::TestMatrixMultiplication(100, 1.0, 2.0, 200.0);
}

TEST(Skotin_A_Multiply_Matrix_Cannon_Seq, Test_Multiplication_500x500) {
  SkotinTBBFuncTests::TestMatrixMultiplication(500, 1.0, 2.0, 1000.0);
}
