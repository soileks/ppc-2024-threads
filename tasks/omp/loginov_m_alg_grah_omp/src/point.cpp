// Copyright 2024 Loginov Maxim

#include "omp/loginov_m_alg_grah_omp/include/point.hpp"

LoginovOmp::LoginovPoint::LoginovPoint() {
  x = 0;
  y = 0;
}

LoginovOmp::LoginovPoint::LoginovPoint(double X, double Y) {
  x = X;
  y = Y;
}

LoginovOmp::LoginovPoint::LoginovPoint(const LoginovOmp::LoginovPoint& p2) {
  x = p2.x;
  y = p2.y;
}

LoginovOmp::LoginovPoint& LoginovOmp::LoginovPoint::operator=(const LoginovOmp::LoginovPoint& p2) {
  if (this == &p2) return *this;
  x = p2.x;
  y = p2.y;
  return *this;
}

bool LoginovOmp::LoginovPoint::operator==(const LoginovOmp::LoginovPoint& p2) const {
  bool res = false;
  if (x == p2.x && y == p2.y) res = true;
  return res;
}

double LoginovOmp::LoginovPoint::angle(const LoginovOmp::LoginovPoint& p) const {
  double dx = p.x - x;
  double dy = p.y - y;
  return atan2(dy, dx);
}

bool LoginovOmp::LoginovPoint::operator()(const LoginovOmp::LoginovPoint& p0,
                                          const LoginovOmp::LoginovPoint& p1) const {
  bool res = false;
  double angle1 = p0.angle((*this));
  double angle2 = p0.angle(p1);

  if (angle1 < angle2)
    res = true;
  else if (angle1 == angle2)
    res = (*this).Distance(p0) < p1.Distance(p0);

  return res;
}

double LoginovOmp::LoginovPoint::Distance(const LoginovOmp::LoginovPoint& p) const {
  return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
}

void LoginovOmp::LoginovPoint::swap(LoginovOmp::LoginovPoint& p2) {
  LoginovOmp::LoginovPoint tmp = (*this);
  (*this) = p2;
  p2 = tmp;
}

int LoginovOmp::LoginovPoint::Compare(const LoginovOmp::LoginovPoint& pivot, const LoginovOmp::LoginovPoint& P) const {
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

LoginovOmp::LoginovPoint LoginovOmp::LoginovPoint::aRandomPoint(double min, double max) {
  std::random_device rd;
  std::default_random_engine gen{rd()};
  std::uniform_real_distribution<double> random(min, max);
  return LoginovOmp::LoginovPoint(random(gen), random(gen));
}