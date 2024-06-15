// Copyright 2024 Kokin Ivan

#include "tbb/kokin_i_image_marking_tbb/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

using namespace std::chrono_literals;

bool imageMarkingTBB::run() {
  // func run
  internal_order_test();
  // func for solution marking component in bin image
  uint32_t scur = 0;
  std::list<uint32_t> vectr;
  std::vector<std::vector<uint32_t *>> maparr;
  maparr.resize(ht);

  tbb::parallel_for(tbb::blocked_range<size_t>(0, ht), [&](const tbb::blocked_range<size_t> &r) {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      maparr[i].resize(wh, nullptr);
    }
  });
  for (size_t i = 0; i < wh; ++i) {
    if (src[0][i] == 0) {
      if (i == 0 || maparr[0][i - 1] == nullptr) {
        vectr.push_back(++scur);
        maparr[0][i] = &vectr.back();
      } else
        maparr[0][i] = maparr[0][i - 1];
    }
  }
  for (size_t i = 1; i < ht; ++i) {
    if (src[i][0] == 0) {
      if (maparr[i - 1][0] == nullptr) {
        vectr.push_back(++scur);
        maparr[i][0] = &vectr.back();
      } else
        maparr[i][0] = maparr[i - 1][0];
    }
    for (size_t j = 1; j < wh; ++j) {
      if (src[i][j] == 0) {
        if (maparr[i - 1][j] == maparr[i][j - 1]) {
          if (maparr[i - 1][j] == nullptr) {
            vectr.push_back(++scur);
            maparr[i][j] = &vectr.back();
          } else
            maparr[i][j] = maparr[i][j - 1];
        } else {
          if (maparr[i - 1][j] == nullptr)
            maparr[i][j] = maparr[i][j - 1];
          else if (maparr[i][j - 1] == nullptr)
            maparr[i][j] = maparr[i - 1][j];
          else {
            *(maparr[i - 1][j]) = *(maparr[i][j - 1]);
            maparr[i][j] = maparr[i][j - 1];
          }
        }
      }
    }
  }

  size_t cnt = 0;
  size_t altr = 0;
  vectr.sort();
  for (auto &label : vectr) {
    if (altr != label) {
      altr = label;
      cnt++;
    }
    label = cnt;
  }

  tbb::parallel_for(tbb::blocked_range2d<size_t>(0, ht, 0, wh), [&](const tbb::blocked_range2d<size_t> &r) {
    for (size_t i = r.rows().begin(); i != r.rows().end(); ++i)
      for (size_t j = r.cols().begin(); j != r.cols().end(); ++j)
        if (maparr[i][j] != nullptr) dest[i][j] = *(maparr[i][j]);
  });
  return true;
}

bool imageMarkingTBB::pre_processing() {
  // func run for init value for input and output
  internal_order_test();
  ht = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];  // init height
  wh = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];  // init width
  src.resize(ht);
  dest.resize(ht);
  for (size_t i = 0; i < ht; ++i) {
    for (size_t j = 0; j < wh; ++j) {
      src[i].push_back(reinterpret_cast<uint8_t *>(taskData->inputs[1])[i * wh + j]);
      dest[i].resize(wh, 0);
    }
  }
  return true;
}

bool imageMarkingTBB::validation() {
  // func validation for check count elements of output
  internal_order_test();
  ht = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];  // init height
  wh = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];  // init width
  return ((uint32_t)ht * (uint32_t)wh == taskData->inputs_count[1] &&
          taskData->inputs_count[1] == taskData->outputs_count[0]);
}

bool imageMarkingTBB::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < ht; ++i) {
    for (size_t j = 0; j < wh; ++j) {
      reinterpret_cast<uint8_t *>(taskData->outputs[0])[i * wh + j] = dest[i][j];
    }
  }
  return true;
}
