// Copyright 2024 Vetoshnikova Ekaterina
#include <oneapi/tbb.h>

#include <thread>

#include "tbb/vetoshnikova_k_convex_hull_bin_image_components/include/ops_tbb.hpp"

using namespace std::chrono_literals;

bool vetoshnikova_tbb::ConstructingConvexHullSeq::pre_processing() {
  internal_order_test();
  h = taskData->inputs_count[0];
  w = taskData->inputs_count[1];
  img.resize(h);
  imgMark.resize(h);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) img[i].push_back(reinterpret_cast<uint8_t*>(taskData->inputs[0])[i * w + j]);

    imgMark[i].resize(w, 0);
  }
  numComponents = 0;

  return true;
}

bool vetoshnikova_tbb::ConstructingConvexHullSeq::validation() {
  internal_order_test();
  return taskData->inputs_count[0] > 0 && taskData->inputs_count[1] > 0;
}

bool vetoshnikova_tbb::ConstructingConvexHullSeq::run() {
  internal_order_test();
  markingComponent();
  for (int i = 0; i < numComponents; ++i) {
    convexHull(i + 2);
  }
  return true;
}

bool vetoshnikova_tbb::ConstructingConvexHullSeq::post_processing() {
  internal_order_test();

  if (!hull.empty()) {
    for (size_t i = 0; i < hull.size(); ++i) reinterpret_cast<int*>(taskData->outputs[0])[i] = hull[i];
  }

  hull.clear();

  return true;
}

int vetoshnikova_tbb::orientation(Point p, Point q, Point r) {
  int val = (q.x - p.x) * (r.y - q.y) - (r.x - q.x) * (q.y - p.y);
  return val;
}

void vetoshnikova_tbb::ConstructingConvexHullSeq::convexHull(int label) {
  std::vector<Point> points;

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (imgMark[i][j] == label) {
        points.push_back({j, i});
      }
    }
  }

  if (!points.empty()) {
    int n = points.size();

    int minX = points[0].x;
    int minIndex = 0;

    for (int i = 1; i < n; ++i) {
      int currX = points[i].x;
      if ((currX < minX) || (currX == minX && points[i].y < points[minIndex].y)) {
        minX = currX;
        minIndex = i;
      }
    }

    std::swap(points[0], points[minIndex]);

    for (int i = 1; i < n; ++i) {
      int j = i;
      while (j > 1 && orientation(points[0], points[j - 1], points[j]) < 0) {
        std::swap(points[j], points[j - 1]);
        j -= 1;
      }
    }

    std::vector<Point> hullPoints;

    hullPoints.emplace_back(points[0]);
    hullPoints.emplace_back(points[1]);

    for (int i = 2; i < n; ++i) {
      while (hullPoints.size() > 1 &&
             orientation(hullPoints[hullPoints.size() - 2], hullPoints[hullPoints.size() - 1], points[i]) < 0) {
        hullPoints.pop_back();
      }

      hullPoints.emplace_back(points[i]);
    }

    int size = hullPoints.size();

    for (int i = 0; i < size; ++i) {
      hull.emplace_back(hullPoints[i].y);
      hull.emplace_back(hullPoints[i].x);
    }
    hull.emplace_back(-1);
  }
}

void vetoshnikova_tbb::ConstructingConvexHullSeq::markingComponent() {
  int label = 2;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (img[i][j] == 1) imgMark[i][j] = 1;
    }
  }

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (imgMark[i][j] == 1) {
        numComponents += 1;
        std::stack<std::pair<int, int>> stack;

        stack.emplace(i, j);

        while (!stack.empty()) {
          int x = stack.top().first;
          int y = stack.top().second;
          stack.pop();

          if (x >= 0 && y >= 0 && x < w && y < h && imgMark[x][y] == 1) {
            imgMark[x][y] = label;

            stack.emplace(x - 1, y);
            stack.emplace(x - 1, y - 1);
            stack.emplace(x - 1, y + 1);
            stack.emplace(x, y - 1);
            stack.emplace(x, y + 1);
            stack.emplace(x + 1, y);
            stack.emplace(x + 1, y - 1);
            stack.emplace(x + 1, y + 1);
          }
        }
        label += 1;
      }
    }
  }
}

bool vetoshnikova_tbb::ConstructingConvexHullTBB::pre_processing() {
  internal_order_test();
  h = taskData->inputs_count[0];
  w = taskData->inputs_count[1];
  img.resize(h);
  imgMark.resize(h);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) img[i].push_back(reinterpret_cast<uint8_t*>(taskData->inputs[0])[i * w + j]);

    imgMark[i].resize(w, 0);
  }
  numComponents = 0;

  return true;
}

bool vetoshnikova_tbb::ConstructingConvexHullTBB::validation() {
  internal_order_test();
  return taskData->inputs_count[0] > 0 && taskData->inputs_count[1] > 0;
}

bool vetoshnikova_tbb::ConstructingConvexHullTBB::run() {
  internal_order_test();
  markingComponent();

  std::vector<std::vector<Point>> PointsComponents;
  PointsComponents.resize(numComponents);

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (imgMark[i][j] != 0) {
        PointsComponents[imgMark[i][j] - 2].push_back({j, i});
      }
    }
  }

  std::vector<std::vector<int>> convexHulls;
  convexHulls.resize(numComponents);

  tbb::parallel_for(tbb::blocked_range<int>(0, numComponents), [&](const tbb::blocked_range<int>& range) {
    for (int i = range.begin(); i != range.end(); ++i) {
      std::vector<int> c = convexHull(PointsComponents[i]);
      int size_hull = c.size();
      convexHulls[i].reserve(size_hull);
      for (int j = 0; j < size_hull; ++j) {
        convexHulls[i].emplace_back(c[j]);
      }
    }
  });

  for (int i = 0; i < numComponents; ++i) {
    int sz = convexHulls[i].size();
    for (int j = 0; j < sz; ++j) {
      hull.emplace_back(convexHulls[i][j]);
    }
  }

  for (int i = 0; i < numComponents; ++i) {
    convexHulls[i].clear();
  }

  for (int i = 0; i < numComponents; ++i) {
    PointsComponents[i].clear();
  }

  return true;
}

bool vetoshnikova_tbb::ConstructingConvexHullTBB::post_processing() {
  internal_order_test();
  if (!hull.empty()) {
    for (size_t i = 0; i < hull.size(); i++) reinterpret_cast<int*>(taskData->outputs[0])[i] = hull[i];
  }

  hull.clear();

  return true;
}

void vetoshnikova_tbb::ConstructingConvexHullTBB::markingComponent() {
  int label = 2;

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (img[i][j] == 1) {
        {
          imgMark[i][j] = 1;
        }
      }
    }
  }

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (imgMark[i][j] == 1) {
        numComponents += 1;
        std::stack<std::pair<int, int>> stack;

        stack.emplace(i, j);

        while (!stack.empty()) {
          int x = stack.top().first;
          int y = stack.top().second;
          stack.pop();

          if (x >= 0 && y >= 0 && x < w && y < h && imgMark[x][y] == 1) {
            imgMark[x][y] = label;
            stack.emplace(x - 1, y);
            stack.emplace(x - 1, y - 1);
            stack.emplace(x - 1, y + 1);
            stack.emplace(x, y - 1);
            stack.emplace(x, y + 1);
            stack.emplace(x + 1, y);
            stack.emplace(x + 1, y - 1);
            stack.emplace(x + 1, y + 1);
          }
        }
        label += 1;
      }
    }
  }
}

std::vector<int> vetoshnikova_tbb::convexHull(std::vector<Point>& points) {
  std::vector<int> hull_thread;

  if (!points.empty()) {
    int n = points.size();

    int minX = points[0].x;
    int minIndex = 0;

    for (int i = 1; i < n; ++i) {
      int currX = points[i].x;
      if ((currX < minX) || (currX == minX && points[i].y < points[minIndex].y)) {
        minX = currX;
        minIndex = i;
      }
    }

    std::swap(points[0], points[minIndex]);

    for (int i = 1; i < n; ++i) {
      int j = i;
      while (j > 1 && orientation(points[0], points[j - 1], points[j]) < 0) {
        std::swap(points[j], points[j - 1]);
        j -= 1;
      }
    }

    std::vector<Point> hullPoints;

    hullPoints.emplace_back(points[0]);
    hullPoints.emplace_back(points[1]);

    for (int i = 2; i < n; ++i) {
      while (hullPoints.size() > 1 &&
             orientation(hullPoints[hullPoints.size() - 2], hullPoints[hullPoints.size() - 1], points[i]) < 0) {
        hullPoints.pop_back();
      }

      hullPoints.emplace_back(points[i]);
    }

    int size = hullPoints.size();

    for (int i = 0; i < size; ++i) {
      hull_thread.emplace_back(hullPoints[i].y);
      hull_thread.emplace_back(hullPoints[i].x);
    }
    hull_thread.emplace_back(-1);
  }
  return hull_thread;
}
