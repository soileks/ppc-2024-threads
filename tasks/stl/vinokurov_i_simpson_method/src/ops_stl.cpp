// Copyright 2024 Vinokurov Ivan
#include "stl/vinokurov_i_simpson_method/include/ops_stl.hpp"

#include <cmath>
#include <functional>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

double fn_simpson(vinokurovIvanSTL::func _fn, double _x0, double _x1, double _y) {
  return _fn(_x0, _y) + 4 * _fn((_x0 + _x1) / 2, _y) + _fn(_x1, _y);
}

double vinokurovIvanSTL::fn_linear(double _x, double _y) { return 9 * _x - _y; }

double vinokurovIvanSTL::fn_exp(double _x, double _y) { return exp(_x - _y); }

double vinokurovIvanSTL::fn_trig(double _x, double _y) { return sin(_x * _y); }

double vinokurovIvanSTL::fn_mul(double _x, double _y) { return _x * _x * _y; }

double vinokurovIvanSTL::fn_other(double _x, double _y) { return (_x + _y) * _y; }

void chunkCalc(vinokurovIvanSTL::func _fn, double _a, double _b, double _c, double _d, int _n, int _begin, int _end,
               double& _result, std::mutex& _mtx) {
  double tmp = 0.0;
  double part1 = static_cast<double>(_b - _a) / _n;
  double part2 = static_cast<double>(_d - _c) / _n;

  for (int i = _begin; i < _end; i++) {
    double a1 = _c + i * part2;
    double a2 = _c + (i + 1) * part2;

    for (int j = 0; j < _n; j++) {
      double b1 = _a + j * part1;
      double b2 = _a + (j + 1) * part1;

      tmp += (part1 * part2 / 36) *
             (fn_simpson(_fn, b1, b2, a1) + 4 * fn_simpson(_fn, b1, b2, (a1 + a2) / 2) + fn_simpson(_fn, b1, b2, a2));
    }
  }

  std::lock_guard<std::mutex> lock(_mtx);
  _result += tmp;
}

bool vinokurovIvanSTL::SimpsonMethodSTL::pre_processing() {
  internal_order_test();
  // Init value for input and output
  auto* inputData = reinterpret_cast<int*>(taskData->inputs[0]);

  a = inputData[0];
  b = inputData[1];
  c = inputData[2];
  d = inputData[3];
  n = inputData[4];

  result = 0.0;

  return true;
}

bool vinokurovIvanSTL::SimpsonMethodSTL::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] == 5 && taskData->outputs_count[0] == 1;
}

bool vinokurovIvanSTL::SimpsonMethodSTL::run() {
  internal_order_test();
  double res{};

  int threadsNumber = std::thread::hardware_concurrency();
  int chunkSize = n / threadsNumber;

  std::vector<std::thread> threads;
  std::mutex threadMutex;

  for (int thrd = 0; thrd < threadsNumber; ++thrd) {
    int begin = thrd * chunkSize;
    int end;
    if (thrd == threadsNumber - 1) {
      end = n;
    } else {
      end = begin + chunkSize;
    }
    threads.emplace_back(chunkCalc, fn, a, b, c, d, n, begin, end, std::ref(res), std::ref(threadMutex));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  result = res;

  return true;
}

bool vinokurovIvanSTL::SimpsonMethodSTL::post_processing() {
  internal_order_test();
  reinterpret_cast<int*>(taskData->outputs[0])[0] = result;
  return true;
}
