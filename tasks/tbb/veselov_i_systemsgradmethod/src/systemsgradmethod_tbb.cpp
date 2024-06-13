// Copyright 2024 Veselov Ilya

#include "tbb/veselov_i_systemsgradmethod/include/systemsgradmethod_tbb.hpp"

#include <tbb/tbb.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>

using namespace std::chrono_literals;

namespace veselov_i_tbb {
double dotProduct(const std::vector<double> &aa, const std::vector<double> &bb) {
  double res = 0.0;
  res = tbb::parallel_reduce(
      tbb::blocked_range<size_t>(0, aa.size()), 0.0,
      [&](const tbb::blocked_range<size_t> &r, double local_res) -> double {
        for (size_t i = r.begin(); i != r.end(); ++i) {
          local_res += aa[i] * bb[i];
        }
        return local_res;
      },
      std::plus<double>());
  return res;
}

std::vector<double> matrixVectorProduct(const std::vector<double> &Aa, const std::vector<double> &xx, int n) {
  std::vector<double> res(n, 0.0);
  tbb::parallel_for(tbb::blocked_range<int>(0, n), [&](const tbb::blocked_range<int> &r) {
    for (int i = r.begin(); i != r.end(); ++i) {
      for (int j = 0; j < n; ++j) {
        res[i] += Aa[i * n + j] * xx[j];
      }
    }
  });
  return res;
}

std::vector<double> SLEgradSolver(const std::vector<double> &Aa, const std::vector<double> &bb, int n,
                                  double tol = 1e-6) {
  std::vector<double> res(n, 0.0);
  std::vector<double> r = bb;
  std::vector<double> p = r;
  std::vector<double> r_old = bb;

  while (true) {
    std::vector<double> Ap = matrixVectorProduct(Aa, p, n);
    double alpha = dotProduct(r, r) / dotProduct(Ap, p);

    tbb::parallel_for(tbb::blocked_range<int>(0, n), [&](const tbb::blocked_range<int> &r) {
      for (int i = r.begin(); i != r.end(); ++i) {
        res[i] += alpha * p[i];
      }
    });

    tbb::parallel_for(tbb::blocked_range<int>(0, n), [&](const tbb::blocked_range<int> &ra) {
      for (int i = ra.begin(); i != ra.end(); ++i) {
        r[i] = r_old[i] - alpha * Ap[i];
      }
    });

    if (std::sqrt(dotProduct(r, r)) < tol) {
      break;
    }
    double beta = dotProduct(r, r) / dotProduct(r_old, r_old);

    tbb::parallel_for(tbb::blocked_range<int>(0, n), [&](const tbb::blocked_range<int> &ra) {
      for (int i = ra.begin(); i != ra.end(); ++i) {
        p[i] = r[i] + beta * p[i];
      }
    });

    r_old = r;
  }
  return res;
}

bool SystemsGradMethodTbb::pre_processing() {
  try {
    internal_order_test();
    A = std::vector<double>(taskData->inputs_count[0]);
    std::copy(reinterpret_cast<double *>(taskData->inputs[0]),
              reinterpret_cast<double *>(taskData->inputs[0]) + taskData->inputs_count[0], A.begin());
    b = std::vector<double>(taskData->inputs_count[1]);
    std::copy(reinterpret_cast<double *>(taskData->inputs[1]),
              reinterpret_cast<double *>(taskData->inputs[1]) + taskData->inputs_count[1], b.begin());
    rows = *reinterpret_cast<int *>(taskData->inputs[2]);
    x = std::vector<double>(rows, 0.0);
  } catch (...) {
    return false;
  }
  return true;
}

bool SystemsGradMethodTbb::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->inputs_count[1] * taskData->inputs_count[1] &&
         taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool SystemsGradMethodTbb::run() {
  try {
    internal_order_test();
    x = SLEgradSolver(A, b, rows);
  } catch (...) {
    return false;
  }
  return true;
}

bool SystemsGradMethodTbb::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < x.size(); ++i) {
    reinterpret_cast<double *>(taskData->outputs[0])[i] = x[i];
  }
  return true;
}

bool checkSolution(const std::vector<double> &Aa, const std::vector<double> &bb, const std::vector<double> &xx,
                   double tol) {
  int n = bb.size();
  std::vector<double> Ax(n, 0.0);
  tbb::parallel_for(tbb::blocked_range<int>(0, n), [&](const tbb::blocked_range<int> &r) {
    for (int i = r.begin(); i != r.end(); ++i) {
      for (int j = 0; j < n; ++j) {
        Ax[i] += Aa[i * n + j] * xx[j];
      }
    }
  });
  for (int i = 0; i < n; ++i) {
    if (std::abs(Ax[i] - bb[i]) > tol) {
      return false;
    }
  }
  return true;
}

std::vector<double> genRandomVector(int size, int maxVal) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distrib(0, maxVal);
  std::vector<double> res(size);
  tbb::parallel_for(tbb::blocked_range<int>(0, size), [&](const tbb::blocked_range<int> &r) {
    for (int i = r.begin(); i != r.end(); ++i) {
      res[i] = distrib(gen);
    }
  });
  return res;
}

std::vector<double> genRandomMatrix(int size, int maxVal) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distrib(0, maxVal);
  std::vector<double> matrix(size * size);

  tbb::parallel_for(tbb::blocked_range<int>(0, size), [&](const tbb::blocked_range<int> &r) {
    for (int i = r.begin(); i != r.end(); ++i) {
      for (int j = i; j < size; ++j) {
        matrix[i * size + j] = distrib(gen);
      }
    }
  });
  tbb::parallel_for(tbb::blocked_range<int>(0, size), [&](const tbb::blocked_range<int> &r) {
    for (int i = r.begin(); i != r.end(); ++i) {
      for (int j = 0; j < i; ++j) {
        matrix[i * size + j] = matrix[j * size + i];
      }
    }
  });
  return matrix;
}
}  // namespace veselov_i_tbb
