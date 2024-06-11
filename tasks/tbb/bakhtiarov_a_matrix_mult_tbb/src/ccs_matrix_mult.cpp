// Copyright 2024 Bakhtiarov Alexander

#include "tbb/bakhtiarov_a_matrix_mult_tbb/include/ccs_matrix_mult.hpp"

#include <tbb/tbb.h>

#include <thread>
#include <vector>

#include "core/task/include/task.hpp"

using namespace std::chrono_literals;
using namespace std;

SparseMatrixMultiTBB::SparseMatrixMultiTBB(std::shared_ptr<ppc::core::TaskData> taskData) : taskData(taskData), result(nullptr) {}

bool SparseMatrixMultiTBB::pre_processing() {
  internal_order_test();
  auto* matrix1 = reinterpret_cast<double*>(taskData->inputs[0]);
  auto* matrix2 = reinterpret_cast<double*>(taskData->inputs[1]);
  numRows1 = taskData->inputs_count[0];
  numCols1 = taskData->inputs_count[1];
  numRows2 = taskData->inputs_count[2];
  numCols2 = taskData->inputs_count[3];
  numRows3 = numRows1;
  numCols3 = numCols2;

  result = new double[numRows1 * numCols2]{0};

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

bool SparseMatrixMultiTBB::validation() {
  internal_order_test();
  return taskData->inputs_count[1] == taskData->inputs_count[2] &&
         taskData->outputs_count[0] == taskData->inputs_count[0] &&
         taskData->outputs_count[1] == taskData->inputs_count[3];
}

bool SparseMatrixMultiTBB::run() {
  internal_order_test();
  values3.clear();
  rows3.clear();
  colPtr3.clear();

  tbb::parallel_for(tbb::blocked_range<int>(0, numCols2), [this](const tbb::blocked_range<int>& range) {
      for (int j = range.begin(); j != range.end(); ++j) {
        for (int i = 0; i < numRows1; i++) {
          double sum = 0.0;
          for (int k = colPtr1[i]; k < colPtr1[i + 1]; k++) {
            int row1 = rows1[k];
            double val1 = values1[k];
            for (int l = colPtr2[j]; l < colPtr2[j + 1]; l++) {
              if (rows2[l] == row1) {
                double val2 = values2[l];
                sum += val1 * val2;
                break;
              }
            }
          }
          if (sum != 0.0) {
            int index = i * numCols2 + j;
            result[index] = sum;
          }
        }
      }
  });

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

bool SparseMatrixMultiTBB::post_processing() {
  internal_order_test();

  auto* out_ptr = reinterpret_cast<double*>(taskData->outputs[0]);
  for (int i = 0; i < numRows3 * numCols3; i++) {
    out_ptr[i] = result[i];
  }

  delete[] result;

  return true;
}
