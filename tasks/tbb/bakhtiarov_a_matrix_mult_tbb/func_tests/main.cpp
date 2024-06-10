// Copyright 2024 Bakhtiarov Alexander
#include <gtest/gtest.h>

#include <vector>

#include "tbb/bakhtiarov_a_matrix_mult_css_tbb/include/ccs_matrix_mult.hpp"

// Тест для проверки корректности размеров матриц
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_sizes) {
  // Задаем размеры матриц
  size_t n1 = 4;
  size_t m1 = 5;
  size_t n2 = 3;
  size_t m2 = 4;

  // Создаем входные данные
  std::vector<double> in1(n1 * m1);
  std::vector<double> in2(n2 * m2);
  std::vector<double> out(n1 * m2);

  // Создаем TaskData для последовательной версии
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

  // Создаем Task для последовательной версии
  SparseTBBMatrixMultiSequential sparseTBBMatrixMultiSequential(taskDataSeq);
  ASSERT_FALSE(sparseTBBMatrixMultiSequential.validation());

  // Добавляем дополнительный тест на проверку корректности размеров матриц
  ASSERT_EQ(sparseTBBMatrixMultiSequential.get_num_rows1(), n1);
  ASSERT_EQ(sparseTBBMatrixMultiSequential.get_num_cols1(), m1);
  ASSERT_EQ(sparseTBBMatrixMultiSequential.get_num_rows2(), n2);
  ASSERT_EQ(sparseTBBMatrixMultiSequential.get_num_cols2(), m2);
}

// Дополнительный тест для проверки корректности умножения матриц
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_multiply) {
  // Задаем размеры матриц
  size_t n1 = 2;
  size_t m1 = 2;
  size_t n2 = 2;
  size_t m2 = 2;

  // Создаем входные данные
  std::vector<double> in1{1, 2, 3, 4};
  std::vector<double> in2{5, 6, 7, 8};
  std::vector<double> out(n1 * m2);

  // Создаем TaskData для последовательной версии
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

  // Создаем Task для последовательной версии
  SparseTBBMatrixMultiSequential sparseTBBMatrixMultiSequential(taskDataSeq);
  ASSERT_TRUE(sparseTBBMatrixMultiSequential.validation());

  // Проверяем корректность умножения
  sparseTBBMatrixMultiSequential.pre_processing();
  sparseTBBMatrixMultiSequential.run();
  sparseTBBMatrixMultiSequential.post_processing();

  ASSERT_EQ(out[0], 19);
  ASSERT_EQ(out[1], 22);
  ASSERT_EQ(out[2], 43);
  ASSERT_EQ(out[3], 50);
}

// Дополнительный тест для проверки вычисления определителя матрицы
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_determinant) {
  // Задаем размеры матриц
  size_t n = 3;

  // Создаем входные данные
  std::vector<double> in{2, 3, 1, 4, 6, 5, 7, 8, 9};

  // Создаем TaskData для последовательной версии
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(nullptr)); // Матрица необходима только для чтения

  // Создаем Task для последовательной версии
  SparseTBBMatrixDeterminant sparseTBBMatrixDeterminant(taskDataSeq);
  ASSERT_TRUE(sparseTBBMatrixDeterminant.validation());

  // Проверяем корректность вычисления определителя
  sparseTBBMatrixDeterminant.pre_processing();
  sparseTBBMatrixDeterminant.run();
  sparseTBBMatrixDeterminant.post_processing();

  ASSERT_EQ(sparseTBBMatrixDeterminant.get_determinant(), -27);
}

// Дополнительный тест для проверки умножения нулевой матрицы
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_multiply_zero_matrix) {
  // Задаем размеры матриц
  size_t n = 3;

  // Создаем входные данные
  std::vector<double> in1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<double> in2(n * n, 0); // Нулевая матрица
  std::vector<double> out(n * n);

  // Создаем TaskData для последовательной версии
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n);
  taskDataSeq->outputs_count.emplace_back(n);

  // Создаем Task для последовательной версии
  SparseTBBMatrixMultiSequential sparseTBBMatrixMultiSequential(taskDataSeq);
  ASSERT_TRUE(sparseTBBMatrixMultiSequential.validation());

  // Проверяем корректность умножения нулевой матрицы
  sparseTBBMatrixMultiSequential.pre_processing();
  sparseTBBMatrixMultiSequential.run();
  sparseTBBMatrixMultiSequential.post_processing();

  // Проверяем, что результат равен нулевой матрице
  for (size_t i = 0; i < out.size(); ++i) {
    ASSERT_EQ(out[i], 0.0);
  }
}

// Дополнительный тест для проверки обратной матрицы
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_inverse_matrix) {
  // Задаем размеры матриц
  size_t n = 3;

  // Создаем входные данные
  std::vector<double> in{2, 3, 1, 4, 6, 5, 7, 8, 9};
  std::vector<double> out(n * n);

  // Создаем TaskData для последовательной версии
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n);
  taskDataSeq->outputs_count.emplace_back(n);

  // Создаем Task для последовательной версии
  SparseTBBMatrixInverse sparseTBBMatrixInverse(taskDataSeq);
  ASSERT_TRUE(sparseTBBMatrixInverse.validation());

  // Проверяем корректность вычисления обратной матрицы
  sparseTBBMatrixInverse.pre_processing();
  sparseTBBMatrixInverse.run();
  sparseTBBMatrixInverse.post_processing();

  // Проверяем корректность обратной матрицы
  std::vector<double> identity{1, 0, 0, 0, 1, 0, 0, 0, 1};
  for (size_t i = 0; i < out.size(); ++i) {
    ASSERT_NEAR(out[i], identity[i], 1e-6); // Используем ASSERT_NEAR для сравнения чисел с плавающей точкой
  }
}

// Дополнительный тест для проверки умножения квадратных матриц
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_multiply_square_matrices) {
  // Задаем размеры матриц
  size_t n = 4;

  // Создаем входные данные
  std::vector<double> in1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  std::vector<double> in2{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  std::vector<double> out(n * n);

  // Создаем TaskData для последовательной версии
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in1.data()));
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in2.data()));
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(n);
  taskDataSeq->outputs_count.emplace_back(n);

  // Создаем Task для последовательной версии
  SparseTBBMatrixMultiSequential sparseTBBMatrixMultiSequential(taskDataSeq);
  ASSERT_TRUE(sparseTBBMatrixMultiSequential.validation());

  // Проверяем корректность умножения квадратных матриц
  sparseTBBMatrixMultiSequential.pre_processing();
  sparseTBBMatrixMultiSequential.run();
  sparseTBBMatrixMultiSequential.post_processing();

  // Проверяем корректность результата
  std::vector<double> expected_out{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  for (size_t i = 0; i < out.size(); ++i) {
    ASSERT_EQ(out[i], expected_out[i]);
  }
}

// Дополнительный тест для проверки корректности вычисления определителя квадратной матрицы
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_determinant_square_matrix) {
  // Задаем размеры матриц
  size_t n = 4;

  // Создаем входные данные
  std::vector<double> in{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

  // Создаем TaskData для последовательной версии
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(n);
  taskDataSeq->inputs_count.emplace_back(n);
   taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(nullptr)); // Матрица необходима только для чтения

  // Создаем Task для последовательной версии
  SparseTBBMatrixDeterminant sparseTBBMatrixDeterminant(taskDataSeq);
  ASSERT_TRUE(sparseTBBMatrixDeterminant.validation());

  // Проверяем корректность вычисления определителя
  sparseTBBMatrixDeterminant.pre_processing();
  sparseTBBMatrixDeterminant.run();
  sparseTBBMatrixDeterminant.post_processing();

  // Определитель квадратной матрицы 4x4 равен 0
  ASSERT_EQ(sparseTBBMatrixDeterminant.get_determinant(), 0);
}

// Дополнительный тест для проверки корректности умножения прямоугольных матриц
TEST(bakhtiarov_a_matrix_mult_css_tbb, test_multiply_rectangular_matrices) {
  // Задаем размеры матриц
  size_t n1 = 3;
  size_t m1 = 2;
  size_t n2 = 2;
  size_t m2 = 4;

  // Создаем входные данные
  std::vector<double> in1{1, 2, 3, 4, 5, 6};
  std::vector<double> in2{1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<double> out(n1 * m2);

  // Создаем TaskData для последовательной версии
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

  // Создаем Task для последовательной версии
  SparseTBBMatrixMultiSequential sparseTBBMatrixMultiSequential(taskDataSeq);
  ASSERT_TRUE(sparseTBBMatrixMultiSequential.validation());

  // Проверяем корректность умножения прямоугольных матриц
  sparseTBBMatrixMultiSequential.pre_processing();
  sparseTBBMatrixMultiSequential.run();
  sparseTBBMatrixMultiSequential.post_processing();

  // Проверяем корректность результата
  std::vector<double> expected_out{9, 12, 19, 26, 39, 54};
  for (size_t i = 0; i < out.size(); ++i) {
    ASSERT_EQ(out[i], expected_out[i]);
  }
}
