// Copyright 2024 Kulagin Aleksandr
#include "omp/kulagin_a_gauss_filter_vert/include/ops_omp.hpp"

#include <cstring>
#include <exception>
#include <omp.h>

bool FilterGaussVerticalTaskOMPKulagin::pre_processing() {
  internal_order_test();
  try {
    // Init value for input and output
    img = reinterpret_cast<uint32_t*>(taskData->inputs[0]);
    kernel = reinterpret_cast<float*>(taskData->inputs[1]);
    w = taskData->inputs_count[0];
    h = taskData->inputs_count[1];
    img_res = std::make_unique<uint32_t[]>(w * h);
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
    return false;
  } catch (...) {
    std::cout << "Something went very terrible!\n";
    return false;
  }
  return true;
}

bool FilterGaussVerticalTaskOMPKulagin::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count.size() == 2 && taskData->inputs.size() == 2 && taskData->outputs.size() == 1 &&
         taskData->inputs_count[0] > 0 && taskData->inputs_count[1] > 0 &&
         taskData->inputs_count == taskData->outputs_count && taskData->inputs[0] != nullptr &&
         taskData->inputs[1] != nullptr;
}

bool FilterGaussVerticalTaskOMPKulagin::run() {
  internal_order_test();
  try {
    #pragma omp for schedule(static)
    for (size_t x = 0; x < w; x++) {
      kulagin_a_gauss::apply_filter_line<true>(w, h, img, kernel, img_res.get(), x);
    }
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
    return false;
  } catch (...) {
    std::cout << "Something went very terrible!\n";
    return false;
  }
  return true;
}

bool FilterGaussVerticalTaskOMPKulagin::post_processing() {
  internal_order_test();
  try {
    std::memcpy(reinterpret_cast<uint32_t*>(taskData->outputs[0]), img_res.get(), w * h * sizeof(uint32_t));
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
    return false;
  } catch (...) {
    std::cout << "Something went very terrible!\n";
    return false;
  }
  return true;
}
