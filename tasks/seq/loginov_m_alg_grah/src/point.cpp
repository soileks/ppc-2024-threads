// Copyright 2024 Loginov Maxim
#include "seq/loginov_m_alg_grah/inc/point.hpp"

Point::Point() {
  x = 0;
  y = 0;
}

Point::Point(double X, double Y) {
  x = X;
  y = Y;
}

Point::Point(const Point& p1) {
  x = p1.x;
  y = p1.y;
}

Point& Point::operator=(const Point& p1) {
  if (this == &p1) return *this;
  x = p1.x;
  y = p1.y;
  return *this;
}

bool Point::operator==(const Point& p1) const {
  bool res = false;
  if (x == p1.x && y == p1.y) res = true;
  return res;
}

double Point::Distance(const Point& p) const { return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y)); }

void Point::Replace(Point& p1) {
  Point tmp = (*this);
  (*this) = p1;
  p1 = tmp;
}

int Point::Compare(const Point& pivot, const Point& P) const {
  int res = 0;
  double x1 = pivot.x - x;
  double y1 = pivot.y - y;
  double x2 = P.x - pivot.x;
  double y2 = P.y - pivot.y;
  double val = x1 * y2 - y1 * x2;

  if (val > 0)
    res = 1;
  else if (val < 0)
    res = -1;
  else {
    float dist1 = (*this).Distance(pivot);
    float dist2 = (*this).Distance(P);

    if (dist1 != 0.0 || dist2 != 0.0) {
      if (dist2 > dist1)
        res = 2;
      else if (dist2 < dist1)
        res = -2;
    }
  }

  return res;
}