// Copyright 2024 Kozlov Mikhail
#include "tbb/kozlov_m_simpson_integral/include/ops_tbb.hpp"

double kozlov_TBB::xy(double x, double y) { return x * y; }
double kozlov_TBB::sinxy(double x, double y) { return std::sin(x * y); }
double kozlov_TBB::linear(double x, double y) { return y * 5 - x * 2; }
double kozlov_TBB::expxy(double x, double y) { return std::exp(x * y); }
double kozlov_TBB::expy_x(double x, double y) { return std::exp(y - x) / 2; }


bool kozlov_TBB::KozlovTasknTBBSequential::pre_processing() {
  internal_order_test();
  auto* tmp = reinterpret_cast<double*>(taskData->inputs[0]);
  x1 = tmp[0], x2 = tmp[1], y1 = tmp[2], y2 = tmp[3];
  res = 0.0;
  f = reinterpret_cast<FUNC>(taskData->inputs[1]);
  n = reinterpret_cast<uint64_t*>(taskData->inputs[2])[0];
  m = reinterpret_cast<uint64_t*>(taskData->inputs[3])[0];
  return true;
}

bool kozlov_TBB::KozlovTasknTBBSequential::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->outputs_count[0] == 1;
}

bool kozlov_TBB::KozlovTasknTBBSequential::run() {
  internal_order_test();
  double h_x = std::abs(x2 - x1) / n;
  double h_y = std::abs(y2 - y1) / m;
  double q;
  double p;
  double x;
  double y;
  double res_ = 0;
  for (int i = 0; i <= n; i++) {
    if (i == 0 || i == n) {
      p = 1;
    } else if (i % 2 == 0) {
      p = 4;
    } else {
      p = 2;
    }
    for (int j = 0; j <= m; j++) {
      if (j == 0 || j == m) {
        q = 1;
      } else if (i % 2 == 0) {
        q = 4;
      } else {
        q = 2;
      }
      x = x1 + i * h_x;
      y = y1 + j * h_y;
      res_ += p * q * f(x, y);
    }
  }

  res_ *= h_x * h_y / 9;
  res = res_;
  return true;
}

bool kozlov_TBB::KozlovTasknTBBSequential::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}

bool kozlov_TBB::KozlovTasknTBBParallel::pre_processing() {
  internal_order_test();
  auto* tmp = reinterpret_cast<double*>(taskData->inputs[0]);
  x1 = tmp[0], x2 = tmp[1], y1 = tmp[2], y2 = tmp[3];
  res = 0.0;
  f = reinterpret_cast<FUNC>(taskData->inputs[1]);
  n = reinterpret_cast<uint64_t*>(taskData->inputs[2])[0];
  m = reinterpret_cast<uint64_t*>(taskData->inputs[3])[0];
  return true;
}

bool kozlov_TBB::KozlovTasknTBBParallel::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == 4 && taskData->outputs_count[0] == 1;
}

bool kozlov_TBB::KozlovTasknTBBParallel::run() {
  internal_order_test();
  double h_x = std::abs(x2 - x1) / n;
  double h_y = std::abs(y2 - y1) / m;
  //tbb::task_scheduler_init init;
  res = tbb::parallel_reduce(
        tbb::blocked_range<int>(0,n),
        0.f,
        [=, *this](const tbb::blocked_range<int>& r, double res_)->double {
            double q;
            double p;
            double x;
            double y;
            for(int i = r.begin(); i!=r.end(); ++i ){
              if (i == 0 || i == n) {
              p = 1;
            } else if (i % 2 == 0) {
              p = 4;
            } else {
              p = 2;
            }
            for (int j = 0; j <= m; j++) {
              if (j == 0 || j == m) {
                q = 1;
              } else if (i % 2 == 0) {
                q = 4;
              } else {
                q = 2;
              }
              x = x1 + i * h_x;
              y = y1 + j * h_y;
              res_ += p * q * f(x, y);
            }
            }
            return res_;
        },
        []( double x, double y )->double {
            return x + y;
        }
    );
  res *= h_x * h_y / 9;
  return true;
}

bool kozlov_TBB::KozlovTasknTBBParallel::post_processing() {
  internal_order_test();
  reinterpret_cast<double*>(taskData->outputs[0])[0] = res;
  return true;
}