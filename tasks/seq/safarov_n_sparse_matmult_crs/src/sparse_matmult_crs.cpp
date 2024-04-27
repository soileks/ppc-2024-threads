// Copyright 2024 Safarov Nurlan
#include "seq/safarov_n_sparse_matmult_crs/include/sparse_matmult_crs.hpp"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

SparseMatrixCRS sparseMatrixTransposeCRS(const SparseMatrixCRS& object) {
  SparseMatrixCRS transportedObject;
  transportedObject.numberOfColumns = object.numberOfRows;
  transportedObject.numberOfRows = object.numberOfColumns;

  transportedObject.pointers.assign(transportedObject.numberOfRows + 1, 0);
  std::vector<std::vector<std::pair<int, double>>> temp(transportedObject.numberOfRows);

  for (int i = 0; i < object.numberOfRows; ++i) {
    for (int j = object.pointers[i]; j < object.pointers[i + 1]; ++j) {
      temp[object.columnIndexes[j]].emplace_back(i, object.values[j]);
    }
  }

  for (int i = 0; i < transportedObject.numberOfRows; ++i) {
    transportedObject.pointers[i + 1] = transportedObject.pointers[i];
    for (auto& t : temp[i]) {
      transportedObject.columnIndexes.push_back(t.first);
      transportedObject.values.push_back(t.second);
      transportedObject.pointers[i + 1]++;
    }
  }

  return transportedObject;
}

bool verifyCRSAttributes(const SparseMatrixCRS& object) {
  int nonZeroCount = object.values.size();
  auto check = size_t(nonZeroCount);

  if (object.pointers.size() != size_t(object.numberOfRows + 1)) {
    return false;
  }
  if (object.pointers[0] != 0) {
    return false;
  }
  if (object.values.size() != check || object.columnIndexes.size() != check ||
      object.pointers[object.numberOfRows] != nonZeroCount) {
    return false;
  }

  for (int i = 0; i < nonZeroCount; ++i) {
    if (object.columnIndexes[i] < 0 || object.columnIndexes[i] >= object.numberOfColumns) {
      return false;
    }
  }

  for (int j = 1; j <= object.numberOfRows; ++j) {
    if (object.pointers[j - 1] > object.pointers[j]) {
      return false;
    }
  }

  return true;
}

bool SparseMatrixMultiplicationCRS::validation() {
  internal_order_test();

  X = reinterpret_cast<SparseMatrixCRS*>(taskData->inputs[0]);
  Y = reinterpret_cast<SparseMatrixCRS*>(taskData->inputs[1]);
  Z = reinterpret_cast<SparseMatrixCRS*>(taskData->outputs[0]);

  if (X == nullptr || Y == nullptr || Z == nullptr) {
    return false;
  }

  if (!verifyCRSAttributes(*X) || !verifyCRSAttributes(*Y)) {
    return false;
  }

  if (taskData->inputs.size() != 2 || taskData->outputs.size() != 1 || !taskData->inputs_count.empty() ||
      !taskData->outputs_count.empty()) {
    return false;
  }

  if (taskData->inputs[0] == nullptr || taskData->inputs[1] == nullptr || taskData->outputs[0] == nullptr) {
    return false;
  }

  return X->numberOfColumns == Y->numberOfRows;
}

bool SparseMatrixMultiplicationCRS::pre_processing() {
  internal_order_test();

  X = reinterpret_cast<SparseMatrixCRS*>(taskData->inputs[0]);
  Y = reinterpret_cast<SparseMatrixCRS*>(taskData->inputs[1]);
  Z = reinterpret_cast<SparseMatrixCRS*>(taskData->outputs[0]);

  *Y = sparseMatrixTransposeCRS(*Y);
  return true;
}

bool SparseMatrixMultiplicationCRS::run() {
  internal_order_test();

  Z->numberOfRows = X->numberOfRows;
  Z->numberOfColumns = X->numberOfRows;

  Z->pointers.assign(Z->numberOfRows + 1, 0);
  std::vector<std::vector<std::pair<int, double>>> temp(Z->numberOfRows);

  for (int i = 0; i < X->numberOfRows; ++i) {
    for (int j = 0; j < Y->numberOfRows; ++j) {
      double v = 0;

      for (int tX = X->pointers[i]; tX < X->pointers[i + 1]; ++tX) {
        for (int tY = Y->pointers[j]; tY < Y->pointers[j + 1]; ++tY) {
          if (X->columnIndexes[tX] == Y->columnIndexes[tY]) {
            v += X->values[tX] * Y->values[tY];
          }
        }
      }
      if (std::fabs(v) > 1e-6) {
        temp[i].emplace_back(j, v);
      }
    }
  }

  for (int i = 0; i < Z->numberOfRows; ++i) {
    Z->pointers[i + 1] = Z->pointers[i];
    for (auto& t : temp[i]) {
      Z->columnIndexes.push_back(t.first);
      Z->values.push_back(t.second);
      Z->pointers[i + 1]++;
    }
  }

  return true;
}

bool SparseMatrixMultiplicationCRS::post_processing() {
  internal_order_test();

  return true;
}
