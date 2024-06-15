// Copyright 2024 Ionova Ekatetina
#include <gtest/gtest.h>

#include <vector>

#include "tbb/ionova_e_sparse_matr_multi_crs_complex_tbb/include/ops_tbb.hpp"

TEST(ionova_e_sparse_matr_multi_crs_complex_tbb, sizes_correct) {
  size_t n1 = 4;
  size_t m1 = 4;
  size_t n2 = 4;
  size_t m2 = 4;

  // Create data
  std::vector<Complex> in1(n1 * m1);

  std::vector<Complex> in2(n2 * m2);

  std::vector<Complex> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixComplexMultiSequentialTbb sparseMatrixComplexMultiSequentialTbb(taskDataSeq);
  ASSERT_EQ(sparseMatrixComplexMultiSequentialTbb.validation(), true);

  // Create data
  std::vector<Complex> in3(n1 * m1);

  std::vector<Complex> in4(n2 * m2);

  std::vector<Complex> out2(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in3.data()));
  taskDataParallel->inputs_count.emplace_back(n1);
  taskDataParallel->inputs_count.emplace_back(m1);
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in4.data()));
  taskDataParallel->inputs_count.emplace_back(n2);
  taskDataParallel->inputs_count.emplace_back(m2);
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataParallel->outputs_count.emplace_back(n1);
  taskDataParallel->outputs_count.emplace_back(m2);

  SparseMatrixComplexMultiParallelTbb sparseMatrixComplexMultiParallelTbb(taskDataParallel);
  ASSERT_EQ(sparseMatrixComplexMultiParallelTbb.validation(), true);
}

TEST(ionova_e_sparse_matr_multi_crs_complex_tbb, sizes_incorrect) {
  size_t n1 = 4;
  size_t m1 = 5;
  size_t n2 = 3;
  size_t m2 = 4;

  // Create data
  std::vector<Complex> in1(n1 * m1);

  std::vector<Complex> in2(n2 * m2);

  std::vector<Complex> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixComplexMultiSequentialTbb sparseMatrixComplexMultiSequentialTbb(taskDataSeq);
  ASSERT_FALSE(sparseMatrixComplexMultiSequentialTbb.validation());

  // Create data
  std::vector<Complex> in3(n1 * m1);

  std::vector<Complex> in4(n2 * m2);

  std::vector<Complex> out2(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in3.data()));
  taskDataParallel->inputs_count.emplace_back(n1);
  taskDataParallel->inputs_count.emplace_back(m1);
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in4.data()));
  taskDataParallel->inputs_count.emplace_back(n2);
  taskDataParallel->inputs_count.emplace_back(m2);
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataParallel->outputs_count.emplace_back(n1);
  taskDataParallel->outputs_count.emplace_back(m2);

  SparseMatrixComplexMultiParallelTbb sparseMatrixComplexMultiParallelTbb(taskDataParallel);
  ASSERT_FALSE(sparseMatrixComplexMultiParallelTbb.validation());
}

TEST(ionova_e_sparse_matr_multi_crs_complex_tbb, zero_matrix) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;
  // Create data
  Complex compNull = {0, 0};
  std::vector<Complex> in1{{3, 2}, {0, 0}, {0, 0}, {0, 0}, {1, -3}, {0, 0}, {0, 0}, {0, 0}, {-4, 1}};
  std::vector<Complex> in2{compNull, compNull, compNull, compNull, compNull, compNull, compNull, compNull, compNull};
  std::vector<Complex> out(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixComplexMultiSequentialTbb sparseMatrixComplexMultiSequentialTbb(taskDataSeq);
  sparseMatrixComplexMultiSequentialTbb.validation();
  sparseMatrixComplexMultiSequentialTbb.pre_processing();
  sparseMatrixComplexMultiSequentialTbb.run();
  sparseMatrixComplexMultiSequentialTbb.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i].real == 0.0 && out[i].imag == 0.0) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);

  // Create data
  std::vector<Complex> out2(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataParallel->inputs_count.emplace_back(n1);
  taskDataParallel->inputs_count.emplace_back(m1);
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataParallel->inputs_count.emplace_back(n2);
  taskDataParallel->inputs_count.emplace_back(m2);
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataParallel->outputs_count.emplace_back(n1);
  taskDataParallel->outputs_count.emplace_back(m2);

  SparseMatrixComplexMultiParallelTbb sparseMatrixComplexMultiParallelTbb(taskDataSeq);
  sparseMatrixComplexMultiParallelTbb.validation();
  sparseMatrixComplexMultiParallelTbb.pre_processing();
  sparseMatrixComplexMultiParallelTbb.run();
  sparseMatrixComplexMultiParallelTbb.post_processing();

  size_t ch = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i].real == 0.0 && out[i].imag == 0.0) {
      ch++;
    }
  }

  ASSERT_EQ(ch, n1 * m2);
}

TEST(ionova_e_sparse_matr_multi_crs_complex_tbb, test_multy) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;

  // Create data
  std::vector<Complex> in1{{3, 2}, {0, 0}, {0, 0}, {0, 0}, {1, -3}, {0, 0}, {0, 0}, {0, 0}, {-4, 1}};
  std::vector<Complex> in2{{0, 0}, {2, -1}, {0, 0}, {0, 0}, {0, 0}, {-5, 2}, {0, 0}, {2, 1}, {0, 0}};
  std::vector<Complex> out(n1 * m2);
  std::vector<Complex> test{{0, 0}, {8, 1}, {0, 0}, {0, 0}, {0, 0}, {1, 17}, {0, 0}, {-9, -2}, {0, 0}};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixComplexMultiSequentialTbb sparseMatrixComplexMultiSequentialTbb(taskDataSeq);
  sparseMatrixComplexMultiSequentialTbb.validation();
  sparseMatrixComplexMultiSequentialTbb.pre_processing();
  sparseMatrixComplexMultiSequentialTbb.run();
  sparseMatrixComplexMultiSequentialTbb.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i].real == test[i].real && out[i].imag == test[i].imag) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);

  // Create data
  std::vector<Complex> out2(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataParallel->inputs_count.emplace_back(n1);
  taskDataParallel->inputs_count.emplace_back(m1);
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataParallel->inputs_count.emplace_back(n2);
  taskDataParallel->inputs_count.emplace_back(m2);
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataParallel->outputs_count.emplace_back(n1);
  taskDataParallel->outputs_count.emplace_back(m2);

  SparseMatrixComplexMultiParallelTbb sparseMatrixComplexMultiParallelTbb(taskDataSeq);
  sparseMatrixComplexMultiParallelTbb.validation();
  sparseMatrixComplexMultiParallelTbb.pre_processing();
  sparseMatrixComplexMultiParallelTbb.run();
  sparseMatrixComplexMultiParallelTbb.post_processing();

  size_t ch = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i].real == test[i].real && out[i].imag == test[i].imag) {
      ch++;
    }
  }

  ASSERT_EQ(ch, n1 * m2);
}

TEST(ionova_e_sparse_matr_multi_crs_complex_tbb, inverse_matrix) {
  size_t n1 = 3;
  size_t m1 = 3;
  size_t n2 = 3;
  size_t m2 = 3;

  // Create data
  std::vector<Complex> in1{{2, -1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 2}, {0, 0}, {2, 1}, {0, 0}};
  std::vector<Complex> in2{{0.4, 0.2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0.4, -0.2}, {0, 0}, {0, -0.5}, {0, 0}};
  std::vector<Complex> out(n1 * m2);
  std::vector<Complex> test{{1, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0}};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n1);
  taskDataSeq->inputs_count.emplace_back(m1);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n2);
  taskDataSeq->inputs_count.emplace_back(m2);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n1);
  taskDataSeq->outputs_count.emplace_back(m2);

  // Create Task
  SparseMatrixComplexMultiSequentialTbb sparseMatrixComplexMultiSequentialTbb(taskDataSeq);
  sparseMatrixComplexMultiSequentialTbb.validation();
  sparseMatrixComplexMultiSequentialTbb.pre_processing();
  sparseMatrixComplexMultiSequentialTbb.run();
  sparseMatrixComplexMultiSequentialTbb.post_processing();

  size_t k = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i].real == test[i].real && out[i].imag == test[i].imag) {
      k++;
    }
  }

  ASSERT_EQ(k, n1 * m2);

  // Create data
  std::vector<Complex> out2(n1 * m2);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataParallel = std::make_shared<ppc::core::TaskData>();
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataParallel->inputs_count.emplace_back(n1);
  taskDataParallel->inputs_count.emplace_back(m1);
  taskDataParallel->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataParallel->inputs_count.emplace_back(n2);
  taskDataParallel->inputs_count.emplace_back(m2);
  taskDataParallel->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataParallel->outputs_count.emplace_back(n1);
  taskDataParallel->outputs_count.emplace_back(m2);

  SparseMatrixComplexMultiParallelTbb sparseMatrixComplexMultiParallelTbb(taskDataSeq);
  sparseMatrixComplexMultiParallelTbb.validation();
  sparseMatrixComplexMultiParallelTbb.pre_processing();
  sparseMatrixComplexMultiParallelTbb.run();
  sparseMatrixComplexMultiParallelTbb.post_processing();

  size_t ch = 0;

  for (size_t i = 0; i < out.size(); ++i) {
    if (out[i].real == test[i].real && out[i].imag == test[i].imag) {
      ch++;
    }
  }

  ASSERT_EQ(ch, n1 * m2);
}
