// Copyright 2024 Bakhtiarov Alexander

#include "stl/bakhtiarov_a_matrix_mult_stl/include/ccs_matrix_mult.hpp"

#include <algorithm>
#include <chrono>
#include <future>
#include <numeric>
#include <thread>
#include <vector>

#include "core/task/include/task.hpp"

using namespace std::chrono_literals;

using namespace std;

bool SparseMatrixMultiSTL::pre_processing() {
  internal_order_test();
  auto* matrix1 = reinterpret_cast<double*>(taskData->inputs[0]);
  auto* matrix2 = reinterpret_cast<double*>(taskData->inputs[1]);
  numRows1 = taskData->inputs_count[0];
  numCols1 = taskData->inputs_count[1];
  numRows2 = taskData->inputs_count[2];
  numCols2 = taskData->inputs_count[3];
  numRows3 = numRows1;
  numCols3 = numCols2;

  result.resize(numRows1 * numCols2, 0);

  int notNullNumbers = 0;
  for (int j = 0; j < numCols1; ++j) {
    colPtr1.push_back(notNullNumbers);
    for (int i = 0; i < numRows1; ++i) {
      int index = i * numCols1 + j;
      if (matrix1[index] != 0) {
        values1.push_back(matrix1[index]);
        rows1.push_back(i);
        notNullNumbers++;
      }
    }
  }
  colPtr1.push_back(notNullNumbers);

  notNullNumbers = 0;
  for (int j = 0; j < numCols2; ++j) {
    colPtr2.push_back(notNullNumbers);
    for (int i = 0; i < numRows2; ++i) {
      int index = i * numCols2 + j;
      if (matrix2[index] != 0) {
        values2.push_back(matrix2[index]);
        rows2.push_back(i);
        notNullNumbers++;
      }
    }
  }
  colPtr2.push_back(notNullNumbers);

  return true;
}

bool SparseMatrixMultiSTL::validation() {
  internal_order_test();
  return taskData->inputs_count[1] == taskData->inputs_count[2] &&
         taskData->outputs_count[0] == taskData->inputs_count[0] &&
         taskData->outputs_count[1] == taskData->inputs_count[3];
}

bool SparseMatrixMultiSTL::run() {
  internal_order_test();

  values3.clear();
  rows3.clear();
  colPtr3.clear();

  values3.resize(numCols2 * numRows1);
  rows3.reserve(numCols2 * numRows1);
  colPtr3.reserve(numCols2 + 1);

  unsigned int num_threads = thread::hardware_concurrency();
  if (num_threads == 0) {
    num_threads = 2;
  }

  auto multiply_columns_range = [&](int start, int end) {
    for (int j = start; j < end; ++j) {
      for (int k = colPtr2[j]; k < colPtr2[j + 1]; k++) {
        int column2 = j;
        int row2 = rows2[k];
        for (int l = colPtr1[row2]; l < colPtr1[row2 + 1]; l++) {
          int row1 = rows1[l];
          double val1 = values1[l];
          double val2 = values2[k];
          int index = row1 * numCols2 + column2;
          result[index] += val1 * val2;
        }
      }
    }
  };

  vector<thread> threads;
  int cols_per_thread = numCols2 / num_threads;
  for (unsigned int t = 0; t < num_threads; ++t) {
    int start = t * cols_per_thread;
    int end = (t == num_threads - 1) ? numCols2 : start + cols_per_thread;
    threads.emplace_back(multiply_columns_range, start, end);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  for (int j = 0; j < numCols2; j++) {
    colPtr3.push_back(values3.size());
    for (int i = 0; i < numRows1; i++) {
      int index = i * numCols2 + j;
      if (result[index] != 0.0) {
        values3.push_back(result[index]);
        rows3.push_back(i);
      }
    }
  }

  colPtr3.push_back(values3.size());

  return true;
}

bool SparseMatrixMultiSTL::post_processing() {
  internal_order_test();

  auto* out_ptr = reinterpret_cast<double*>(taskData->outputs[0]);
  for (int i = 0; i < numRows1 * numCols2; i++) {
    out_ptr[i] = result[i];
  }

  return true;
}
