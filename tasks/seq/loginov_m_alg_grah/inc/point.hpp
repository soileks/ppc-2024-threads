// Copyright 2024 Loginov Maxim
#pragma once

#include <cmath>

struct Point {
  double x;
  double y;

  Point();
  Point(double X, double Y);
  Point(const Point& p2);

  Point& operator=(const Point& p2);
  bool operator==(const Point& p2) const;

  double Distance(const Point& p) const;
  void Replace(Point& p2);
  int Compare(const Point& pivot, const Point& P) const;
};