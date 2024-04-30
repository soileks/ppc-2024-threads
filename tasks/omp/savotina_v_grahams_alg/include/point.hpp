// Copyright 2024 Savotina Valeria
#pragma once

#include <cmath>
#include <random>

namespace SavotinaOmp {
struct SavotinaPoint {
  double x;
  double y;

  SavotinaPoint();
  SavotinaPoint(double X, double Y);
  SavotinaPoint(const SavotinaPoint& p2);

  SavotinaPoint& operator=(const SavotinaPoint& p2);
  bool operator==(const SavotinaPoint& p2) const;
  bool operator()(const SavotinaPoint& p0, const SavotinaPoint& p1) const;

  double angle(const SavotinaPoint& p) const;
  double Distance(const SavotinaPoint& p) const;
  void swap(SavotinaPoint& p2);
  int Compare(const SavotinaPoint& pivot, const SavotinaPoint& P) const;
  static SavotinaPoint aRandomPoint(double min, double max);
};
}  // namespace SavotinaOmp