// Copyright 2024 Kurdina Julia
#include "tbb/kurdina_j_linear_filter/include/ops_tbb.hpp"

#include <tbb/tbb.h>

#include <algorithm>
#include <thread>

using namespace std::chrono_literals;

uint8_t HorizontalSplitTBB_parallel::NewColor(int i, int j) {
  int r = ker_size / 2;
  int res = 0;
  int im_ind_n;
  int im_ind_m;
  for (int k = -r; k < r + 1; k++) {
    for (int q = -r; q < r + 1; q++) {
      if (k + i <= 0)
        im_ind_n = 0;
      else if (k + i >= n)
        im_ind_n = n - 1;
      else
        im_ind_n = k + i;

      if (q + j <= 0)
        im_ind_m = 0;
      else if (q + j >= m)
        im_ind_m = m - 1;
      else
        im_ind_m = q + j;
      res += image[im_ind_n * m + im_ind_m] * gauss_kernel[(k + 1) * ker_size + (q + 1)];
    }
  }
  res = (uint8_t)(std::clamp(res, 0, 255));
  return res;
}

bool HorizontalSplitTBB_parallel::pre_processing() {
  internal_order_test();
  n = reinterpret_cast<int *>(taskData->inputs[1])[0];
  m = reinterpret_cast<int *>(taskData->inputs[1])[1];
  for (int i = 0; i < n * m; i++) {
    input_.push_back(reinterpret_cast<uint8_t *>(taskData->inputs[0])[i]);
    image.push_back(0);
    result_im.push_back(0);
  }
  for (int i = 0; i < ker_size * ker_size; i++) {
    gauss_kernel.push_back(reinterpret_cast<uint8_t *>(taskData->inputs[2])[i]);
  }
  for (int i = 0; i < n * m; i++) {
    image[i] = input_[i];
  }
  return true;
}

bool HorizontalSplitTBB_parallel::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->outputs_count[0] && taskData->inputs_count[2] == 9;
}

bool HorizontalSplitTBB_parallel::run() {
  internal_order_test();
  oneapi::tbb::parallel_for(oneapi::tbb::blocked_range<int>(0, n), [&](const oneapi::tbb::blocked_range<int> &r) {
    for (int i = r.begin(); i < r.end(); i++) {
      for (int j = 0; j < m; j++) {
        result_im[i * m + j] = NewColor(i, j);
      }
    }
  });

  return true;
}

bool HorizontalSplitTBB_parallel::post_processing() {
  internal_order_test();
  for (int i = 0; i < n * m; i++) {
    reinterpret_cast<uint8_t *>(taskData->outputs[0])[i] = result_im[i];
  }
  image.clear();
  result_im.clear();
  input_.clear();
  gauss_kernel.clear();
  return true;
}

uint8_t HorizontalSplitTBB_seq::NewColor(int i, int j) {
  int r = ker_size / 2;
  int res = 0;
  int im_ind_n;
  int im_ind_m;
  for (int k = -r; k < r + 1; k++) {
    for (int q = -r; q < r + 1; q++) {
      if (k + i <= 0)
        im_ind_n = 0;
      else if (k + i >= n)
        im_ind_n = n - 1;
      else
        im_ind_n = k + i;

      if (q + j <= 0)
        im_ind_m = 0;
      else if (q + j >= m)
        im_ind_m = m - 1;
      else
        im_ind_m = q + j;
      res += image[im_ind_n * m + im_ind_m] * gauss_kernel[(k + 1) * ker_size + (q + 1)];
    }
  }
  res = (uint8_t)(std::clamp(res, 0, 255));
  return res;
}

bool HorizontalSplitTBB_seq::pre_processing() {
  internal_order_test();
  n = reinterpret_cast<int *>(taskData->inputs[1])[0];
  m = reinterpret_cast<int *>(taskData->inputs[1])[1];
  for (int i = 0; i < n * m; i++) {
    input_.push_back(reinterpret_cast<uint8_t *>(taskData->inputs[0])[i]);
    image.push_back(0);
    result_im.push_back(0);
  }
  for (int i = 0; i < ker_size * ker_size; i++) {
    gauss_kernel.push_back(reinterpret_cast<uint8_t *>(taskData->inputs[2])[i]);
  }
  for (int i = 0; i < n * m; i++) {
    image[i] = input_[i];
  }
  return true;
}

bool HorizontalSplitTBB_seq::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->outputs_count[0] && taskData->inputs_count[2] == 9;
}

bool HorizontalSplitTBB_seq::run() {
  internal_order_test();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      result_im[i * m + j] = NewColor(i, j);
    }
  }
  return true;
}

bool HorizontalSplitTBB_seq::post_processing() {
  internal_order_test();
  for (int i = 0; i < n * m; i++) {
    reinterpret_cast<uint8_t *>(taskData->outputs[0])[i] = result_im[i];
  }
  image.clear();
  result_im.clear();
  input_.clear();
  gauss_kernel.clear();
  return true;
}