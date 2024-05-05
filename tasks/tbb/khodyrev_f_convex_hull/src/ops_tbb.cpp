// Copyright 2024 Khodyrev Fedor
#include "tbb/khodyrev_f_convex_hull/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

#include <algorithm>
#include <iostream>
#include <vector>

bool khodyrev_tbb::isWhite(const std::vector<int>& image, int width, int x, int y) { return image[y * width + x] == 1; }

bool khodyrev_tbb::KhodyrevTaskTBB::pre_processing() {
  internal_order_test();
  try {
    height_in = taskData->inputs_count[0];
    width_in = taskData->inputs_count[1];
    height_out = taskData->outputs_count[0];
    width_out = taskData->outputs_count[1];
    for (int i = 0; i < height_in * width_in; i++) {
      image.push_back(reinterpret_cast<int*>(taskData->inputs[0])[i]);
    }
    for (int i = 0; i < height_out * width_out; i++) {
      result.push_back(reinterpret_cast<int*>(taskData->outputs[0])[i]);
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

bool khodyrev_tbb::KhodyrevTaskTBB::validation() {
  internal_order_test();
  return taskData->inputs_count[0] == taskData->outputs_count[0] ||
         taskData->inputs_count[1] == taskData->outputs_count[1];
}

bool khodyrev_tbb::KhodyrevTaskTBB::run() {
  internal_order_test();
  try {
    tbb::parallel_for(tbb::blocked_range2d<int>(0, height_in, 0, width_in),
                      [&](const tbb::blocked_range2d<int>& range) {
                        for (int y = range.rows().begin(); y < range.rows().end(); ++y) {
                          for (int x = range.cols().begin(); x < range.cols().end(); ++x) {
                            if (isWhite(image, width_in, x, y)) {
                              Pixel pixel;
                              pixel.x = x;
                              pixel.y = y;
                              pixels.push_back(pixel);
                            }
                          }
                        }
                      });

    start_point = pixels[0];
    for (const Pixel& pixel : pixels) {
      if (pixel.y < start_point.y || (pixel.y == start_point.y && pixel.x < start_point.x)) {
        start_point = pixel;
      }
    }

    std::sort(pixels.begin(), pixels.end(), [&](const Pixel& p1, const Pixel& p2) -> bool {
      int orientation =
          (p1.x - start_point.x) * (p2.y - start_point.y) - (p2.x - start_point.x) * (p1.y - start_point.y);
      if (orientation == 0) return (p1.x + p1.y) < (p2.x + p2.y);
      return orientation > 0;
    });

    std::stack<Pixel> hull;
    hull.push(pixels[0]);
    hull.push(pixels[1]);

    for (size_t i = 2; i < pixels.size(); ++i) {
      while (hull.size() >= 2) {
        Pixel p1 = hull.top();
        hull.pop();
        Pixel p2 = hull.top();
        if ((p2.x - p1.x) * (pixels[i].y - p1.y) - (p2.y - p1.y) * (pixels[i].x - p1.x) < 0) {
          hull.push(p1);
          break;
        }
      }
      hull.push(pixels[i]);
    }

    while (!hull.empty()) {
      Pixel p = hull.top();
      hull.pop();
      result[p.y * width_out + p.x] = 1;
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

bool khodyrev_tbb::KhodyrevTaskTBB::post_processing() {
  internal_order_test();
  try {
    for (int i = 0; i < height_out * width_out; i++) {
      reinterpret_cast<int*>(taskData->outputs[0])[i] = result[i];
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}