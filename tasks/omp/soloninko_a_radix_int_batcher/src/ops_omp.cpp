// Copyright 2024 Soloninko Andrey
#include "omp/soloninko_a_radix_int_batcher/include/ops_omp.hpp"

#include <omp.h>

namespace SoloninkoOMPBatcher {
void sol_fill_missing(std::vector<int> &res, std::vector<int> p1, std::vector<int> p2, size_t i, size_t j, size_t k) {
  if (j >= p1.size()) {
    for (size_t l = k; l < p2.size(); l += 2) {
      res[i] = p2[l];
      i++;
    }
  } else {
    for (size_t l = j; l < p1.size(); l += 2) {
      res[i] = p1[l];
      i++;
    }
  }
}

void sol_fill_res(std::vector<int> &res, std::vector<int> p1, std::vector<int> p2, size_t &i, size_t &j, size_t &k) {
  for (i = 0; (j < p1.size()) && (k < p2.size()); i++) {
    if (p1[j] <= p2[k]) {
      res[i] = p1[j];
      j += 2;
    } else {
      res[i] = p2[k];
      k += 2;
    }
  }
}

std::vector<int> sol_merge(std::vector<int> vec1, std::vector<int> vec2) {
  std::vector<int> res(vec1.size() + vec2.size());
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;

  for (i = 0, j = 0, k = 0; (j < vec1.size()) && (k < vec2.size()); i += 2, j++, k++) {
    res[i] = vec1[j];
    res[i + 1] = vec2[k];
  }

  for (i = 0; i < res.size() - 1; i++) {
    if (res[i] > res[i + 1]) {
      std::swap(res[i], res[i + 1]);
    }
  }

  return res;
}

std::vector<int> soll_even(const std::vector<int> &p1, const std::vector<int> &p2) {
  std::vector<int> res(p1.size() / 2 + p2.size() / 2 + p1.size() % 2 + p2.size() % 2);
  size_t i = 0;
  size_t i1 = 0;
  size_t i2 = 0;

  sol_fill_res(res, p1, p2, i, i1, i2);

  sol_fill_missing(res, p1, p2, i, i1, i2);

  return res;
}

std::vector<int> sol_odd(const std::vector<int> &p1, const std::vector<int> &p2) {
  std::vector<int> res(p1.size() / 2 + p2.size() / 2);
  size_t i = 0;
  size_t i1 = 1;
  size_t i2 = 1;

  sol_fill_res(res, p1, p2, i, i1, i2);

  sol_fill_missing(res, p1, p2, i, i1, i2);

  return res;
}

std::vector<int> sol_batcher_sort(const std::vector<int> &p1, const std::vector<int> &p2) {
  std::vector<int> even_vec = soll_even(p1, p2);

  std::vector<int> odd_vec = sol_odd(p1, p2);

  std::vector<int> res = sol_merge(even_vec, odd_vec);

  return res;
}

void sol_fill_missing_omp(std::vector<int> &res, std::vector<int> p1, std::vector<int> p2, size_t i, size_t j,
                          size_t k) {
  if (j >= p1.size()) {
    for (size_t l = k; l < p2.size(); l += 2) {
      res[i] = p2[l];
      i++;
    }
  } else {
    for (size_t l = j; l < p1.size(); l += 2) {
      res[i] = p1[l];
      i++;
    }
  }
}

void sol_fill_res_omp(std::vector<int> &res, std::vector<int> p1, std::vector<int> p2, size_t &i, size_t &j,
                      size_t &k) {
  for (i = 0; (j < p1.size()) && (k < p2.size()); i++) {
    if (p1[j] <= p2[k]) {
      res[i] = p1[j];
      j += 2;
    } else {
      res[i] = p2[k];
      k += 2;
    }
  }
}

std::vector<int> sol_merge_omp(std::vector<int> vec1, std::vector<int> vec2) {
  std::vector<int> res(vec1.size() + vec2.size());
  // size_t i = 0;
  // size_t j = 0;
  // size_t k = 0;

  for (size_t i = 0, j = 0, k = 0; (j < vec1.size()) && (k < vec2.size()); i += 2, j++, k++) {
    res[i] = vec1[j];
    res[i + 1] = vec2[k];
  }

#pragma omp parallel for
  for (int i = 0; i < static_cast<int>(res.size()) - 1; i++) {
    if (res[i] > res[i + 1]) {
      std::swap(res[i], res[i + 1]);
    }
  }

  return res;
}

std::vector<int> soll_even_omp(const std::vector<int> &p1, const std::vector<int> &p2) {
  std::vector<int> res(p1.size() / 2 + p2.size() / 2 + p1.size() % 2 + p2.size() % 2);
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;

  while ((j < p1.size()) && (k < p2.size())) {
    if (p1[j] <= p2[k]) {
      res[i] = p1[j];
      j += 2;
    } else {
      res[i] = p2[k];
      k += 2;
    }
    i++;
  }

  if (j >= p1.size()) {
    for (size_t l = k; l < p2.size(); l += 2) {
      res[i] = p2[l];
      i++;
    }
  } else {
    for (size_t l = j; l < p1.size(); l += 2) {
      res[i] = p1[l];
      i++;
    }
  }

  return res;
}

std::vector<int> sol_odd_omp(const std::vector<int> &p1, const std::vector<int> &p2) {
  std::vector<int> res(p1.size() / 2 + p2.size() / 2);
  size_t i = 0;
  size_t j = 1;
  size_t k = 1;

  sol_fill_res_omp(res, p1, p2, i, j, k);

  sol_fill_missing_omp(res, p1, p2, i, j, k);

  return res;
}

std::vector<int> sol_batcher_sort_omp(const std::vector<int> &p1, const std::vector<int> &p2) {
  std::vector<int> even_vec = soll_even_omp(p1, p2);

  std::vector<int> odd_vec = sol_odd_omp(p1, p2);

  std::vector<int> res = sol_merge_omp(even_vec, odd_vec);

  return res;
}

bool TestTaskOMPParallelBatcherSoloninko::pre_processing() {
  internal_order_test();
  // Init value for input and output
  in_vec = std::vector<int>(taskData->inputs_count[0]);

  auto *tmp_ptr_A = reinterpret_cast<int *>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    in_vec[i] = tmp_ptr_A[i];
  }

  p1.resize(in_vec.size() / 2);
  p2.resize(in_vec.size() / 2);

  for (size_t i = 0; i < (in_vec.size() / 2); i++) {
    p1[i] = in_vec[i];
    p2[i] = in_vec[(in_vec.size() / 2) + i];
  }

  std::sort(p1.begin(), p1.end());
  std::sort(p2.begin(), p2.end());

  return true;
}

bool TestTaskOMPParallelBatcherSoloninko::validation() {
  internal_order_test();
  // Check count elements of output

  return std::is_sorted(p1.begin(), p1.end()) && std::is_sorted(p2.begin(), p2.end());
}

bool TestTaskOMPParallelBatcherSoloninko::run() {
  internal_order_test();

  res = sol_batcher_sort_omp(p1, p2);
  return true;
}

bool TestTaskOMPParallelBatcherSoloninko::post_processing() {
  internal_order_test();
  std::copy(res.begin(), res.end(), reinterpret_cast<int *>(taskData->outputs[0]));
  return true;
}

bool TaskOMPSequentialBatcherSoloninko::pre_processing() {
  internal_order_test();
  // Init value for input and output
  in_vec = std::vector<int>(taskData->inputs_count[0]);

  auto *tmp_ptr_A = reinterpret_cast<int *>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    in_vec[i] = tmp_ptr_A[i];
  }

  p1.resize(in_vec.size() / 2);
  p2.resize(in_vec.size() / 2);

  for (size_t i = 0; i < (in_vec.size() / 2); i++) {
    p1[i] = in_vec[i];
    p2[i] = in_vec[(in_vec.size() / 2) + i];
  }

  std::sort(p1.begin(), p1.end());
  std::sort(p2.begin(), p2.end());

  return true;
}

bool TaskOMPSequentialBatcherSoloninko::validation() {
  internal_order_test();
  // Check count elements of output

  return std::is_sorted(p1.begin(), p1.end()) && std::is_sorted(p2.begin(), p2.end());
}

bool TaskOMPSequentialBatcherSoloninko::run() {
  internal_order_test();

  res = sol_batcher_sort(p1, p2);
  return true;
}

bool TaskOMPSequentialBatcherSoloninko::post_processing() {
  internal_order_test();
  std::copy(res.begin(), res.end(), reinterpret_cast<int *>(taskData->outputs[0]));
  return true;
}
}  // namespace SoloninkoOMPBatcher
