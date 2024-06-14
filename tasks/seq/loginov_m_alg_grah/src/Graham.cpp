// Copyright 2024 Loginov Maxim
#include "seq/loginov_m_alg_grah/inc/Graham.hpp"

void QuickSort(std::vector<Point>& pArr, int left, int right) {
  if (left > right) return;

  int piv = 0;
  int arrSize = right - left + 1;
  int L = left;
  int R = right;

  if ((arrSize % 2) == 0)
    piv = (arrSize / 2 - 1) + left;
  else
    piv = std::trunc(arrSize / 2) + left;

  Point pivot = pArr[piv];

  while (L <= R) {
    while (!(pArr[0].Compare(pivot, pArr[L]) >= 0)) ++L;

    while (!(pArr[0].Compare(pivot, pArr[R]) <= 0)) --R;

    if (L <= R) {
      pArr[L].Replace(pArr[R]);
      ++L;
      --R;
    }
  }

  QuickSort(pArr, left, R);
  QuickSort(pArr, L, right);
}

Point MinPoint(const std::vector<Point>& pArr) {
  double minX = pArr[0].x;
  double minY = 0;
  std::stack<int> S;
  S.push(0);
  int pArrSize = pArr.size();

  for (int i = 1; i < pArrSize; ++i) {
    double Xi = pArr[i].x;
    if (Xi < minX) {
      minX = Xi;
      while (!S.empty()) S.pop();
      S.push(i);
    } else if (Xi == minX) {
      S.push(i);
    }
  }

  minY = pArr[S.top()].y;
  S.pop();
  while (!S.empty()) {
    double Yi = pArr[S.top()].y;
    if (Yi < minY) minY = Yi;
    S.pop();
  }

  return Point(minX, minY);
}

int PointPosition(const Point& p, const std::vector<Point>& pArr) {
  int pp = 0;
  int pArrSize = pArr.size();
  for (int i = 0; i < pArrSize; ++i) {
    if (pArr[i].x == p.x && pArr[i].y == p.y) {
      pp = i;
      break;
    }
  }
  return pp;
}

std::vector<Point> MinConvexHull(std::vector<Point> pArr) {
  if (pArr.size() < 3) return pArr;

  std::vector<Point> mch;
  int ind1 = 0;
  int ind2 = 1;
  int arrSize = pArr.size();

  while (pArr[ind1] == pArr[ind2]) {
    ++ind2;
    if (ind2 == arrSize) break;
  }

  mch.push_back(pArr[ind1]);
  if (ind2 != arrSize) {
    mch.push_back(pArr[ind2]);

    arrSize = pArr.size();
    int val = 0;
    int mchSize = 0;

    for (int i = ind2 + 1; i < arrSize; ++i) {
      mchSize = mch.size();
      val = mch[mchSize - 2].Compare(mch[mchSize - 1], pArr[i]);

      if ((val == -1) || (val == 2)) {
        mch.erase(mch.end() - 1);
        if (mch.size() < 2) mch.push_back(pArr[i]);
        --i;
      } else if (val == 1) {
        mch.push_back(pArr[i]);
      }
    }
  }

  return mch;
}

bool GrahamsAlgorithmSequential::pre_processing() {
  internal_order_test();

  pointsArr = std::vector<Point>(taskData->inputs_count[0]);

  auto* pArray = reinterpret_cast<Point*>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    pointsArr[i] = pArray[i];
  }

  minConvexHull = pointsArr;
  return true;
}

bool GrahamsAlgorithmSequential::validation() {
  internal_order_test();

  return (taskData->outputs_count[0] <= taskData->inputs_count[0]);
}

bool GrahamsAlgorithmSequential::run() {
  internal_order_test();

  if (pointsArr.empty()) return true;

  Point P0 = MinPoint(pointsArr);
  int p0 = PointPosition(P0, pointsArr);

  minConvexHull[0].Replace(minConvexHull[p0]);
  QuickSort(minConvexHull, 1, minConvexHull.size() - 1);

  minConvexHull = MinConvexHull(minConvexHull);
  return true;
}

bool GrahamsAlgorithmSequential::post_processing() {
  internal_order_test();

  std::copy(minConvexHull.begin(), minConvexHull.end(), reinterpret_cast<Point*>(taskData->outputs[0]));
  return true;
}