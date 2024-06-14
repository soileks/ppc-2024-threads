// Copyright 2024 Mironov Ilya
#include "tbb/mironov_i_sparse_crs/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

#include <cstring>
#include <random>
#include <vector>
const double EPS = 1e-6;

mironov_tbb::MatrixCRS::MatrixCRS(int n, int nz) : N(n), NZ(nz) {
  Value.resize(nz);
  Col.resize(nz);
  RowIndex.resize(n + 1);
}

mironov_tbb::MatrixCRS::MatrixCRS(const double* matrix, int n, int m, bool transpose) {
  int index;
  if (transpose) {
    N = m;
    m = n;
  } else {
    N = n;
  }
  RowIndex.reserve(N + 1);
  NZ = 0;
  for (int i = 0; i < N; i++) {
    RowIndex[i] = NZ;
    for (int j = 0; j < m; j++) {
      index = transpose ? i + N * j : i * m + j;
      if (std::fabs(matrix[index]) >= EPS) {
        Value.push_back(matrix[index]);
        Col.push_back(j);
        NZ++;
      }
    }
  }
  RowIndex[N] = NZ;
}

mironov_tbb::MatrixCRS Multiplicate2(const mironov_tbb::MatrixCRS& A, const mironov_tbb::MatrixCRS& BT, int m) {
  int N = A.N;
  auto* columns = new std::vector<int>[N];
  auto* values = new std::vector<double>[N];
  int* row_index = new int[N + 1];
  memset(row_index, 0, sizeof(int) * N);
  tbb::parallel_for(tbb::blocked_range<int>(0, N), [&](const tbb::blocked_range<int>& range) {
    int j;
    int k;
    int* temp = new int[m];
    for (int i = range.begin(); i < range.end(); i++) {
      memset(temp, -1, m * sizeof(*temp));
      int ind1 = A.RowIndex[i];
      int ind2 = A.RowIndex[i + 1];
      for (k = ind1; k < ind2; k++) {
        temp[A.Col[k]] = k;
      }
      for (j = 0; j < BT.N; j++) {
        double sum = 0;
        int ind3 = BT.RowIndex[j];
        int ind4 = BT.RowIndex[j + 1];
        for (k = ind3; k < ind4; k++) {
          int aind = temp[BT.Col[k]];
          if (aind != -1) {
            sum += A.Value[aind] * BT.Value[k];
          }
        }
        if (fabs(sum) > EPS) {
          columns[i].push_back(j);
          values[i].push_back(sum);
          row_index[i]++;
        }
      }
    }
    delete[] temp;
  });
  // }
  int NZ = 0;
  int i;
  for (i = 0; i < N; i++) {
    int tmp = row_index[i];
    row_index[i] = NZ;
    NZ += tmp;
  }
  row_index[N] = NZ;
  mironov_tbb::MatrixCRS C(N, NZ);
  int count = 0;
  for (i = 0; i < N; i++) {
    int size = columns[i].size();
    memcpy(C.Col.data() + count, columns[i].data(), size * sizeof(*columns[i].data()));
    memcpy(C.Value.data() + count, values[i].data(), size * sizeof(*values[i].data()));
    count += size;
  }
  memcpy(C.RowIndex.data(), row_index, (N + 1) * sizeof(*row_index));
  delete[] row_index;
  delete[] columns;
  delete[] values;
  return C;
}

bool MironovITBB::pre_processing() {
  internal_order_test();
  M = taskData->inputs_count[1];
  K = taskData->inputs_count[3];
  A = mironov_tbb::MatrixCRS(reinterpret_cast<double*>(taskData->inputs[0]), taskData->inputs_count[0], M, false);
  BT = mironov_tbb::MatrixCRS(reinterpret_cast<double*>(taskData->inputs[1]), taskData->inputs_count[2], K, true);
  c_out = reinterpret_cast<double*>(taskData->outputs[0]);
  return true;
}

bool MironovITBB::validation() {
  internal_order_test();
  // Check count elements of output
  return (taskData->inputs[0] != nullptr) && (taskData->inputs_count[0] != 0u) && (taskData->inputs[1] != nullptr) &&
         (taskData->inputs_count[1] != 0u) && (taskData->outputs[0] != nullptr) && (taskData->outputs_count[0] != 0u);
}

void PrintCRSMatrix(const mironov_tbb::MatrixCRS& matrix, int m) {
  for (int i = 0; i < matrix.N; i++) {
    int j = 0;
    for (int c_j = matrix.RowIndex[i]; c_j < matrix.RowIndex[i + 1]; c_j++) {
      while (j < matrix.Col[c_j]) {
        std::cout << "0 ";
        j++;
      }
      std::cout << matrix.Value[c_j] << " ";
      j++;
    }
    while (j < m) {
      std::cout << "0 ";
      j++;
    }
    std::cout << std::endl;
  }
}

bool MironovITBB::run() {
  internal_order_test();
  C = Multiplicate2(A, BT, M);
  return true;
}

bool MironovITBB::post_processing() {
  internal_order_test();
  for (int row = 0; row < C.N; row++) {
    for (int c_j = C.RowIndex[row]; c_j < C.RowIndex[row + 1]; c_j++) {
      c_out[row * K + C.Col[c_j]] = C.Value[c_j];
    }
  }
  return true;
}

void MironovITBB::genrateSparseMatrix(double* matrix, int sz, double ro) {
  int nz = sz * ro;
  std::uniform_int_distribution<int> distribution(0, sz - 1);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> rand_r(-10.0, 10.0);
  for (int i = 0; i < nz; i++) {
    matrix[distribution(gen)] = rand_r(gen);
  }
}

void MironovITBB::genrateEdMatrix(double* matrix, int n) {
  for (int i = 0; i < n; i++) {
    matrix[i * n + i] = 1.0;
  }
}
