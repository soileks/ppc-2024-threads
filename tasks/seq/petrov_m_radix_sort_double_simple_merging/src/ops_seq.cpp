// Copyright 2024 Petrov Maksim

#include "seq/petrov_m_radix_sort_double_simple_merging/include/ops_seq.hpp"

#include <thread>

using namespace std::chrono_literals;

void PetrovRadixSortDoubleSequential::PetrovCountSort(double* in, double* out, int len, int exp) {
  auto* buf = reinterpret_cast<unsigned char*>(in);
  int count[256] = {0};
  for (int i = 0; i < len; i++) {
    count[buf[8 * i + exp]]++;
  }
  int sum = 0;
  for (int i = 0; i < 256; i++) {
    int temp = count[i];
    count[i] = sum;
    sum += temp;
  }
  for (int i = 0; i < len; i++) {
    out[count[buf[8 * i + exp]]] = in[i];
    count[buf[8 * i + exp]]++;
  }
}

bool PetrovRadixSortDoubleSequential::PetrovCountSortSigns(const double* in, double* out, int len) {
  bool positiveFlag = false;
  bool negativeFlag = false;
  int firstNegativeIndex = -1;
  // int firstPositiveIndex = -1;
  for (int i = 0; i < len; i++) {
    if (positiveFlag && negativeFlag) {
      break;
    }
    if (in[i] < 0 && !negativeFlag) {
      negativeFlag = true;
      firstNegativeIndex = i;
    }
    if (in[i] > 0 && !positiveFlag) {
      positiveFlag = true;
      // firstPositiveIndex = i;
    }
  }
  if (positiveFlag && negativeFlag) {
    bool forward = false;
    int j = len - 1;
    for (int i = 0; i < len; i++) {
      out[i] = in[j];
      if (forward) {
        j++;
      } else {
        j--;
      }
      if (j == firstNegativeIndex - 1 && !forward) {
        j = 0;
        forward = true;
      }
    }
    return true;
  }
  if (!positiveFlag) {
    for (int i = len - 1, j = 0; i >= 0; i--, j++) {
      out[j] = in[i];
    }
    return true;
  }
  return false;
}

std::vector<double> PetrovRadixSortDoubleSequential::PetrovRadixSort(const std::vector<double>& data) {
  int len = static_cast<int>(data.size());
  std::vector<double> in = data;
  std::vector<double> out(data.size());

  for (int i = 0; i < 4; i++) {
    PetrovCountSort(in.data(), out.data(), len, 2 * i);
    PetrovCountSort(out.data(), in.data(), len, 2 * i + 1);
  }
  if (!PetrovCountSortSigns(in.data(), out.data(), len)) {
    in.swap(out);
  }
  return out;
}

bool PetrovRadixSortDoubleSequential::pre_processing() {
  internal_order_test();
  try {
    data_size = taskData->inputs_count[0];
    while (!sort.empty()) {
      sort.pop_back();
    }
    for (int i = 0; i < data_size; i++) {
      sort.push_back((reinterpret_cast<double*>((taskData->inputs[0])))[i]);
    }
  } catch (...) {
    std::cout << "\n";
    std::cout << "Double radix sort error";
    std::cout << "\n";
    return false;
  }
  return true;
}
bool PetrovRadixSortDoubleSequential::validation() {
  internal_order_test();
  // Check count elements of output
  return ((taskData->inputs_count[0] > 1) && (taskData->outputs_count[0] == taskData->inputs_count[0]));
}

bool PetrovRadixSortDoubleSequential::run() {
  internal_order_test();
  try {
    sort = (PetrovRadixSort(sort));
  } catch (...) {
    std::cout << "\n";
    std::cout << "Double radix sort error";
    std::cout << "\n";
    return false;
  }
  return true;
}

bool PetrovRadixSortDoubleSequential::post_processing() {
  internal_order_test();
  try {
    auto* outputs = reinterpret_cast<double*>(taskData->outputs[0]);
    for (int i = 0; i < data_size; i++) {
      outputs[i] = sort[i];
    }
  } catch (...) {
    std::cout << "\n";
    std::cout << "Double radix sort error";
    std::cout << "\n";
    return false;
  }
  return true;
}