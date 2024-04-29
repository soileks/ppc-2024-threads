// Copyright 2024 Savotina Valeria

#include "omp/savotina_v_grahams_alg/include/point.hpp"

SavotinaOmp::SavotinaPoint::SavotinaPoint() {
  x = 0;
  y = 0;
}

SavotinaOmp::SavotinaPoint::SavotinaPoint(double X, double Y) {
  x = X;
  y = Y;
}

SavotinaOmp::SavotinaPoint::SavotinaPoint(const SavotinaOmp::SavotinaPoint& p2) {
  x = p2.x;
  y = p2.y;
}

SavotinaOmp::SavotinaPoint& SavotinaOmp::SavotinaPoint::operator=(const SavotinaOmp::SavotinaPoint& p2) {
  if (this == &p2) return *this;
  x = p2.x;
  y = p2.y;
  return *this;
}

bool SavotinaOmp::SavotinaPoint::operator==(const SavotinaOmp::SavotinaPoint& p2) const {
  bool res = false;
  if (x == p2.x && y == p2.y) res = true;
  return res;
}

double SavotinaOmp::SavotinaPoint::Distance(const SavotinaOmp::SavotinaPoint& p) const {
  return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
}

void SavotinaOmp::SavotinaPoint::swap(SavotinaOmp::SavotinaPoint& p2) {
  SavotinaOmp::SavotinaPoint tmp = (*this);
  (*this) = p2;
  p2 = tmp;
}

int SavotinaOmp::SavotinaPoint::Compare(const SavotinaOmp::SavotinaPoint& pivot,
                                        const SavotinaOmp::SavotinaPoint& P) const {
  int res = 0;  // ==
  double x1 = pivot.x - x;
  double y1 = pivot.y - y;
  double x2 = P.x - pivot.x;
  double y2 = P.y - pivot.y;
  double val = x1 * y2 - y1 * x2;

  if (val > 0)
    res = 1;  // 2 upper 1 | 2 > 1
  else if (val < 0)
    res = -1;  // 2 downer 1 | 2 < 1
  else {
    float dist1 = (*this).Distance(pivot);
    float dist2 = (*this).Distance(P);

    if (dist1 != 0.0 || dist2 != 0.0) {
      if (dist2 > dist1)
        res = 2;  // 2 > 1
      else if (dist2 < dist1)
        res = -2;  // 2 < 1
    }
  }

  return res;
}

SavotinaOmp::SavotinaPoint SavotinaOmp::SavotinaPoint::aRandomPoint(double min, double max) {
  size_t seed = 1000;
  std::default_random_engine gen(seed);
  std::uniform_real_distribution<double> random(min, max);
  return SavotinaOmp::SavotinaPoint(random(gen), random(gen));
}
