// Copyright 2024 Prokofev Kirill
#include "tbb/prokofev_k_convexHullForBinImage/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <set>
#include <thread>

using namespace std::chrono_literals;

bool prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb::pre_processing() {
  internal_order_test();
  try {
    auto* input = reinterpret_cast<int*>(taskData->inputs[0]);
    width = taskData->inputs_count[0];
    height = taskData->inputs_count[1];
    resSize = taskData->inputs_count[2];
    img.resize(height * width);
    for (int i = 0; i < width * height; i++) {
      img[i] = input[i];
    }
    res.resize(resSize);
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  return true;
}

bool prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb::validation() {
  internal_order_test();
  return taskData->inputs_count.size() == 3 && taskData->inputs_count[0] > 0 && taskData->inputs_count[1] > 0 &&
         taskData->inputs_count[2] > 0 && taskData->inputs[0] != nullptr;
}

bool prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb::run() {
  internal_order_test();
  try {
    std::vector<int> local_image = FindComponents(img, width, height);
    int count_components = FindCountComponents(local_image);
    int num_threads = tbb::this_task_arena::max_concurrency();
    int components_per_thread = count_components / num_threads;
    int extra_components = count_components % num_threads;
    std::vector<std::vector<int>> local_results(num_threads, std::vector<int>());
    tbb::parallel_for(0, num_threads, [&](int thread_num) {
      int start_index = thread_num * components_per_thread;
      int end_index = start_index + components_per_thread;
      if (thread_num == num_threads - 1) {
        end_index += extra_components;
      }
      for (int i = start_index; i < end_index; ++i) {
        int component_index = i + 1;
        std::vector<int> points = RemoveExtraPoints(local_image, width, height, component_index);
        std::vector<int> ch = GrahamAlgorithm(points);
        local_results[thread_num].insert(local_results[thread_num].end(), ch.begin(), ch.end());
        local_results[thread_num].emplace_back(-1);
      }
    });
    int k = 0;
    for (int i = 0; i < num_threads; ++i) {
      if (!local_results[i].empty()) {
        for (size_t j = 0; j < local_results[i].size(); ++j) {
          res[k] = local_results[i][j];
          k++;
        }
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return false;
  }

  return true;
}

bool prokofev_k_convexHull_Tbb::BinaryImageConvexHullTbb::post_processing() {
  internal_order_test();
  try {
    std::memcpy(reinterpret_cast<int*>(taskData->outputs[0]), res.data(), res.size() * sizeof(int));
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  return true;
}

void prokofev_k_convexHull_Tbb::FloodFill(std::vector<int>* image, int height, int width, int yStart, int xStart,
                                          int label) {
  std::queue<prokofev_k_convexHull_Tbb::Point> tasks;
  tasks.emplace(xStart, yStart);
  while (!tasks.empty()) {
    int x = tasks.front().x;
    int y = tasks.front().y;
    tasks.pop();
    if (x >= 0 && y >= 0 && y < height && x < width && image->at(y * width + x) == 1) {
      (*image)[y * width + x] = label;
      tasks.emplace(x - 1, y - 1);
      tasks.emplace(x - 1, y);
      tasks.emplace(x - 1, y + 1);
      tasks.emplace(x, y + 1);
      tasks.emplace(x + 1, y + 1);
      tasks.emplace(x + 1, y);
      tasks.emplace(x, y - 1);
      tasks.emplace(x + 1, y - 1);
    }
  }
}

std::vector<int> prokofev_k_convexHull_Tbb::FindComponents(const std::vector<int>& image, int width, int height) {
  std::vector<int> image_with_components = image;
  int label = 2;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (image_with_components[i * width + j] == 1) {
        prokofev_k_convexHull_Tbb::FloodFill(&image_with_components, height, width, i, j, label);
        label++;
      }
    }
  }
  tbb::parallel_for(tbb::blocked_range2d<int>(0, height, 0, width), [&](const tbb::blocked_range2d<int>& r) {
    for (int i = r.rows().begin(); i != r.rows().end(); ++i) {
      for (int j = r.cols().begin(); j != r.cols().end(); ++j) {
        if (image_with_components[i * width + j] != 0) {
          image_with_components[i * width + j]--;
        }
      }
    }
  });
  return image_with_components;
}

int prokofev_k_convexHull_Tbb::FindCountComponents(const std::vector<int>& image) {
  std::set<int> labels;
  for (size_t i = 0; i < image.size(); ++i) {
    if (image[i] != 0) {
      labels.insert(image[i]);
    }
  }
  return labels.size();
}

int prokofev_k_convexHull_Tbb::FindCountPointsInComponent(const std::vector<int>& image) {
  int count_points = 0;
  for (size_t i = 0; i < image.size(); ++i) {
    if (image[i] != 0) {
      count_points++;
    }
  }
  return count_points;
}

std::vector<int> prokofev_k_convexHull_Tbb::RemoveExtraPoints(const std::vector<int>& image, int width, int height,
                                                              int label) {
  std::vector<int> local_image(image);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (image[i * width + j] == label) {
        if ((j > 0) && (j < width - 1)) {
          if ((i == 0) || (i == height - 1)) {
            if ((image[i * width + j - 1] == label) && (image[i * width + j + 1] == label)) {
              local_image[i * width + j] = 0;
            }
          }
          if ((i > 0) && (i < height - 1)) {
            if (((image[i * width + j - 1] == label) && (image[i * width + j + 1] == label)) ||
                ((image[(i + 1) * width + j] == label) && (image[(i - 1) * width + j] == label))) {
              local_image[i * width + j] = 0;
            }
          }
          continue;
        }
        if ((i > 0) && (i < height - 1)) {
          if ((j == 0) || (j == width - 1)) {
            if ((image[(i - 1) * width + j] == label) && (image[(i + 1) * width + j] == label)) {
              local_image[i * width + j] = 0;
            }
          }
          if ((j > 0) && (j < width - 1)) {
            if (((image[i * width + j - 1] == label) && (image[i * width + j + 1] == label)) ||
                ((image[(i + 1) * width + j] == label) && (image[(i - 1) * width + j] == label))) {
              local_image[i * width + j] = 0;
            }
          }
        }
      } else {
        local_image[i * width + j] = 0;
      }
    }
  }
  int size = prokofev_k_convexHull_Tbb::FindCountPointsInComponent(local_image);
  std::vector<int> points(size * 2);
  int k = 0;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (local_image[i * width + j] != 0) {
        points[k] = j;
        k++;
        points[k] = i;
        k++;
      }
    }
  }
  return points;
}

int prokofev_k_convexHull_Tbb::Cross(int x1, int y1, int x2, int y2, int x3, int y3) {
  return ((x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1));
}

void prokofev_k_convexHull_Tbb::Sort(std::vector<int>* points, int xMin, int yMin) {
  int size = points->size() / 2;
  for (int i = 1; i < size; ++i) {
    int j = i;
    while ((j > 0) && (Cross(xMin, yMin, (*points)[2 * j - 2], (*points)[2 * j - 1], (*points)[2 * j],
                             (*points)[2 * j + 1]) < 0)) {
      int temp = (*points)[2 * j - 2];
      (*points)[2 * j - 2] = (*points)[2 * j];
      (*points)[2 * j] = temp;
      temp = (*points)[2 * j - 1];
      (*points)[2 * j - 1] = (*points)[2 * j + 1];
      (*points)[2 * j + 1] = temp;
      j--;
    }
  }
}

std::vector<int> prokofev_k_convexHull_Tbb::GrahamAlgorithm(std::vector<int> points) {
  std::vector<int> result;
  int num_points = points.size() / 2;
  if (num_points > 1) {
    int x_min = points[0];
    int y_min = points[1];
    int min_index = 0;
    for (size_t i = 2; i < points.size(); i += 2) {
      if (points[i] < x_min || (points[i] == x_min && points[i + 1] < y_min)) {
        x_min = points[i];
        y_min = points[i + 1];
        min_index = i;
      }
    }
    int temp = points[min_index];
    points[min_index] = points[num_points * 2 - 2];
    points[num_points * 2 - 2] = temp;
    temp = points[min_index + 1];
    points[min_index + 1] = points[num_points * 2 - 1];
    points[num_points * 2 - 1] = temp;
    points.pop_back();
    points.pop_back();
    prokofev_k_convexHull_Tbb::Sort(&points, x_min, y_min);
    result.emplace_back(x_min);
    result.emplace_back(y_min);
    result.emplace_back(points[0]);
    result.emplace_back(points[1]);
    for (size_t i = 2; i < points.size(); i += 2) {
      int result_size = result.size();
      int x1 = result[result_size - 4];
      int y1 = result[result_size - 3];
      int x2 = result[result_size - 2];
      int y2 = result[result_size - 1];
      int x3 = points[i];
      int y3 = points[i + 1];

      int rot = prokofev_k_convexHull_Tbb::Cross(x1, y1, x2, y2, x3, y3);
      if (rot == 0) {
        result[result_size - 2] = x3;
        result[result_size - 1] = y3;
      } else if (rot < 0) {
        while (prokofev_k_convexHull_Tbb::Cross(result[(result.size()) - 4], result[(result.size()) - 3],
                                                result[(result.size()) - 2], result[(result.size()) - 1], x3, y3) < 0)
          result.pop_back(), result.pop_back();
        result.emplace_back(x3);
        result.emplace_back(y3);
      } else {
        result.emplace_back(x3);
        result.emplace_back(y3);
      }
    }
  } else {
    result.resize(2);
    result[0] = points[0];
    result[1] = points[1];
  }
  return result;
}

std::vector<int> prokofev_k_convexHull_Tbb::ConvertImageToVector(std::vector<std::vector<int>> notVecImg, int width,
                                                                 int height) {
  std::vector<int> newImg(width * height);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      newImg[i * width + j] = notVecImg[i][j];
    }
  }
  return newImg;
}

std::vector<int> prokofev_k_convexHull_Tbb::GenerateImgForPerfTests(int componentCount) {
  int width = 8;
  int height = 8;
  std::vector<int> nullRow = {0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<int> row = {0, 0, 1, 1, 1, 1, 0, 0};
  std::vector<std::vector<int>> notVecImg(height * componentCount + componentCount);
  for (int i = 0; i < componentCount; i++) {
    for (int j = 0; j < height + 1; j++) {
      if (j != 8) {
        notVecImg[i * (height + 1) + j] = row;
      } else {
        notVecImg[i * (height + 1) + j] = nullRow;
      }
    }
  }
  return prokofev_k_convexHull_Tbb::ConvertImageToVector(notVecImg, width, notVecImg.size());
}