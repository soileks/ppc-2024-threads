// Copyright 2024 Loginov Maxim

#include "omp/loginov_m_alg_grah_omp/include/ops_omp.hpp"

namespace LoginovOmp {

// A search minimum point in point's array (min x, then min y)
LoginovPoint LoginovMinPoint(const std::vector<LoginovPoint>& pointArr) {
  double minX = pointArr[0].x;
  double minY = 0;
  std::stack<int> S;
  S.push(0);
  int pArrSize = pointArr.size();

  // Search min x
  for (int i = 1; i < pArrSize; ++i) {
    double Xi = pointArr[i].x;
    if (Xi < minX) {
      minX = Xi;
      while (!S.empty()) S.pop();
      S.push(i);
    } else if (Xi == minX) {
      S.push(i);
    }
  }

  // Search min y
  minY = pointArr[S.top()].y;
  S.pop();
  while (!S.empty()) {
    double Yi = pointArr[S.top()].y;
    if (Yi < minY) minY = Yi;
    S.pop();
  }

  return LoginovPoint(minX, minY);  // Anonymous object
}

// Determining the number (index) of a point in the array
int LoginovPointPositionSeq(const LoginovPoint& p, const std::vector<LoginovPoint>& pointArr) {
  int pp = 0;
  int pArrSize = pointArr.size();
  for (int i = 0; i < pArrSize; ++i) {
    if (pointArr[i].x == p.x && pointArr[i].y == p.y) {
      pp = i;
      break;
    }
  }
  return pp;
}

// Creating a minimum convex hull
std::vector<LoginovPoint> LoginovMinConvexHull(std::vector<LoginovPoint> pointArr) {
  if (pointArr.size() < 3) return pointArr;

  std::vector<LoginovPoint> mch;
  int ind1 = 0;
  int ind2 = 1;
  int arrSize = pointArr.size();

  while (pointArr[ind1] == pointArr[ind2]) {
    ++ind2;
    if (ind2 == arrSize) break;
  }

  mch.push_back(pointArr[ind1]);
  if (ind2 != arrSize) {
    mch.push_back(pointArr[ind2]);

    arrSize = pointArr.size();
    int val = 0;
    int mchSize = 0;

    for (int i = ind2 + 1; i < arrSize; ++i) {
      mchSize = mch.size();
      val = mch[mchSize - 2].Compare(mch[mchSize - 1], pointArr[i]);

      if ((val == -1) || (val == 2))  // (val == -1) 2 < 1 || (val == 2) dist2 > dist1, 2 > 1
      {
        mch.erase(mch.end() - 1);
        if (mch.size() < 2) mch.push_back(pointArr[i]);
        --i;
      } else if (val == 1)  // (val == 1)  2 > 1
      {
        mch.push_back(pointArr[i]);
      }
    }
  }

  return mch;
}

bool LoginovGrahAlgSequential::pre_processing() {
  internal_order_test();

  // Init value for input and output
  pointsArr = std::vector<LoginovPoint>(taskData->inputs_count[0]);

  auto* pArray = reinterpret_cast<LoginovPoint*>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    pointsArr[i] = pArray[i];
  }

  minConvexHull = pointsArr;
  return true;
}

bool LoginovGrahAlgSequential::validation() {
  internal_order_test();

  // Check count elements of output
  return (taskData->outputs_count[0] <= taskData->inputs_count[0]);
}

bool LoginovGrahAlgSequential::run() {
  internal_order_test();

  if (pointsArr.empty()) return true;

  // Step 1: search the minimum point P0
  LoginovPoint P0 = LoginovOmp::LoginovMinPoint(pointsArr);
  int p0 = LoginovOmp::LoginovPointPositionSeq(P0, pointsArr);

  // Step 2: sort all points except P0
  minConvexHull[0].swap(minConvexHull[p0]);
  std::sort(minConvexHull.begin() + 1, minConvexHull.end(),
            [&P0](LoginovPoint& p1, LoginovPoint& p2) { return p1(P0, p2); });

  // Step 3: build a minimum convex hull
  minConvexHull = LoginovOmp::LoginovMinConvexHull(minConvexHull);

  return true;
}

bool LoginovGrahAlgSequential::post_processing() {
  internal_order_test();

  std::copy(minConvexHull.begin(), minConvexHull.end(), reinterpret_cast<LoginovPoint*>(taskData->outputs[0]));
  return true;
}

// Determining the number (index) of a point in the array
int LoginovPointPositionOMP(const LoginovPoint& p, const std::vector<LoginovPoint>& pointArr) {
  int pp = -1;
  int pArrSize = pointArr.size();

#pragma omp parallel for
  for (int i = 0; i < pArrSize; ++i) {
    if (pointArr[i].x == p.x && pointArr[i].y == p.y) {
#pragma omp critical
      {
        if (pp == -1) pp = i;
      }
    }
  }
  return pp;
}

void LoginovSortOMP(std::vector<LoginovPoint>& vec, const LoginovPoint& P0) {
  int num_threads = omp_get_max_threads();
  size_t size = vec.size();

#pragma omp parallel for num_threads(num_threads)
  for (int i = 0; i < num_threads; ++i) {
    size_t start = (size_t)i * size / num_threads;
    size_t end = (size_t)(i + 1) * size / num_threads;
    std::sort(vec.begin() + start, vec.begin() + end,
              [P0](const LoginovPoint& p1, const LoginovPoint& p2) { return p1(P0, p2); });
  }

  for (int i = 1; i < num_threads; ++i) {
    size_t start = (size_t)i * size / num_threads;
    size_t end = (size_t)(i + 1) * size / num_threads;
    std::inplace_merge(vec.begin(), vec.begin() + start, vec.begin() + end,
                       [P0](const LoginovPoint& p1, const LoginovPoint& p2) { return p1(P0, p2); });
  }
}

bool LoginovGrahAlgOmpParallel::pre_processing() {
  internal_order_test();

  // Init value for input and output
  pointsArr = std::vector<LoginovPoint>(taskData->inputs_count[0]);

  auto* pArray = reinterpret_cast<LoginovPoint*>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    pointsArr[i] = pArray[i];
  }

  minConvexHull = pointsArr;
  return true;
}

bool LoginovGrahAlgOmpParallel::validation() {
  internal_order_test();

  // Check count elements of output
  return (taskData->outputs_count[0] <= taskData->inputs_count[0]);
}

bool LoginovGrahAlgOmpParallel::run() {
  internal_order_test();

  if (pointsArr.empty()) return true;

  // Step 1: search the minimum point P0
  LoginovPoint P0 = LoginovOmp::LoginovMinPoint(pointsArr);
  int p0 = LoginovOmp::LoginovPointPositionOMP(P0, pointsArr);

  // Step 2: sort all points except P0
  minConvexHull[0].swap(minConvexHull[p0]);

  minConvexHull.erase(minConvexHull.begin());
  LoginovSortOMP(minConvexHull, P0);
  minConvexHull.insert(minConvexHull.begin(), P0);

  // Step 3: build a minimum convex hull
  minConvexHull = LoginovOmp::LoginovMinConvexHull(minConvexHull);

  return true;
}

bool LoginovGrahAlgOmpParallel::post_processing() {
  internal_order_test();

  std::copy(minConvexHull.begin(), minConvexHull.end(), reinterpret_cast<LoginovPoint*>(taskData->outputs[0]));
  return true;
}

std::vector<LoginovPoint> LoginovRandomPoints(double leftBorder, double rightBorder, size_t size) {
  std::vector<LoginovPoint> arrPoints(size);
  for (LoginovPoint& value : arrPoints) value = LoginovPoint::aRandomPoint(leftBorder, rightBorder);
  return arrPoints;
}

}  // namespace LoginovOmp