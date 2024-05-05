// Copyright 2024 Kozlov Mikhail
#include "stl/kozlov_m_simpson_integral/include/ops_stl.hpp"

double kozlov_stl::xy(double x, double y) { return x * y; }
double kozlov_stl::sinxy(double x, double y) { return std::sin(x * y); }
double kozlov_stl::linear(double x, double y) { return y * 5 - x * 2; }
double kozlov_stl::expxy(double x, double y) { return std::exp(x * y); }
double kozlov_stl::expy_x(double x, double y) { return std::exp(y - x) / 2; }

bool kozlov_stl::KozlovTaskSequential::pre_processing() {
  internal_order_test();
  auto* tmp = reinterpret_cast<double*>(taskData->inputs[0]);
  x1 = tmp[0], x2 = tmp[1], y1 = tmp[2], y2 = tmp[3];
  res = 0.0;
  f = reinterpret_cast<FUNC>(taskData->inputs[1]);
  n = reinterpret_cast<uint64_t*>(taskData->inputs[2])[0];
  m = reinterpret_cast<uint64_t*>(taskData->inputs[3])[0];
  return true;
}

bool kozlov_stl::KozlovTaskSequential::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->outputs_count[0] == 1;
}

bool kozlov_stl::KozlovTaskSequential::run() {
  internal_order_test();
  h_x = std::abs(x2 - x1) / n;
  h_y = std::abs(y2 - y1) / m;
  std::mutex mtx;
  auto Int = [&mtx, this](const uint32_t start, const uint32_t end) {
    for (uint64_t i = start; i <= end; i++) {
      double q;
      double p;
      double x;
      double y;
      if (i == 0 || i == n) {
        p = 1;
      } else if (i % 2 == 0) {
        p = 4;
      } else {
        p = 2;
      }
      for (uint64_t j = 0; j <= m; j++) {
        if (j == 0 || j == m) {
          q = 1;
        } else if (i % 2 == 0) {
          q = 4;
        } else {
          q = 2;
        }
        x = x1 + i * h_x;
        y = y1 + j * h_y;
        std::scoped_lock lck(mtx);
        res += p * q * f(x, y);
      }
    }
  };
  std::thread th1(Int, 0, n / 4);
  std::thread th2(Int, n / 4 + 1, n / 2);
  std::thread th3(Int, n / 2 + 1, 3 * n / 4);
  std::thread th4(Int, 3 * n / 4 + 1, n);
  th1.join();
  th2.join();
  th3.join();
  th4.join();

  res *= h_x * h_y / 9;
  return true;
}

bool kozlov_stl::KozlovTaskSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}