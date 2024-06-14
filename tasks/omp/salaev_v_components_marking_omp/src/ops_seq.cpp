// Copyright 2024 Salaev Vladislav

#include "omp/salaev_v_components_marking_omp/include/ops_seq.hpp"

using namespace SalaevOMP;

bool ImageMarkingSeq::validation() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  return (height * width == taskData->inputs_count[1] && taskData->inputs_count[1] == taskData->outputs_count[0]);
}

bool ImageMarkingSeq::pre_processing() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  source.resize(height);
  destination.resize(height);
  for (uint32_t i = 0; i < height; ++i) {
    for (uint32_t j = 0; j < width; ++j)
      source[i].push_back(reinterpret_cast<uint8_t *>(taskData->inputs[1])[i * width + j]);
    destination[i].resize(width, 0);
  }
  return true;
}

bool ImageMarkingSeq::run() {
  internal_order_test();
  int nextLabel = 1;
  std::vector<int> labels(height * width, -1);
  std::vector<std::set<int>> equivalence(height * width);

  // First pass
  for (int i = 0; i < static_cast<int>(height); ++i) {
    for (int j = 0; j < static_cast<int>(width); ++j) {
      if (source[i][j] == 0) {
        std::set<int> neighboringLabels;
        if (i > 0 && labels[(i - 1) * width + j] != -1) neighboringLabels.insert(labels[(i - 1) * width + j]);
        if (j > 0 && labels[i * width + (j - 1)] != -1) neighboringLabels.insert(labels[i * width + (j - 1)]);

        if (neighboringLabels.empty()) {
          labels[i * width + j] = nextLabel;
          equivalence[nextLabel].insert(nextLabel);
          ++nextLabel;
        } else {
          int smallestLabel = *neighboringLabels.begin();
          labels[i * width + j] = smallestLabel;
          for (auto label : neighboringLabels) {
            equivalence[label].insert(smallestLabel);
            equivalence[smallestLabel].insert(label);
          }
        }
      }
    }
  }

  // Resolve equivalences
  for (size_t i = 0; i < equivalence.size(); ++i) {
    if (!equivalence[i].empty()) {
      int smallestLabel = *equivalence[i].begin();
      for (auto label : equivalence[i]) {
        std::replace(labels.begin(), labels.end(), label, smallestLabel);
      }
    }
  }

  // Flatten labels
  resolve_labels(labels);

  // Second pass
  for (int i = 0; i < static_cast<int>(height); ++i) {
    for (int j = 0; j < static_cast<int>(width); ++j) {
      if (source[i][j] == 0) {
        destination[i][j] = labels[i * width + j];
      }
    }
  }

  return true;
}

bool ImageMarkingSeq::post_processing() {
  internal_order_test();
  for (uint32_t i = 0; i < height; ++i)
    for (uint32_t j = 0; j < width; ++j)
      reinterpret_cast<uint32_t *>(taskData->outputs[0])[i * width + j] = destination[i][j];
  return true;
}

void ImageMarkingSeq::resolve_labels(std::vector<int> &labels) {
  std::unordered_map<int, int> label_map;
  int new_label = 1;
  for (auto &label : labels) {
    if (label != -1) {
      if (label_map.find(label) == label_map.end()) {
        label_map[label] = new_label++;
      }
      label = label_map[label];
    }
  }
}

bool ImageMarkingOmp::validation() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  return (height * width == taskData->inputs_count[1] && taskData->inputs_count[1] == taskData->outputs_count[0]);
}

bool ImageMarkingOmp::pre_processing() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  source.resize(height);
  destination.resize(height);
  for (uint32_t i = 0; i < height; ++i) {
    for (uint32_t j = 0; j < width; ++j)
      source[i].push_back(reinterpret_cast<uint8_t *>(taskData->inputs[1])[i * width + j]);
    destination[i].resize(width, 0);
  }
  return true;
}

bool ImageMarkingOmp::run() {
  internal_order_test();
  int nextLabel = 1;
  std::vector<int> labels(height * width, -1);
  std::vector<std::set<int>> equivalence(height * width);

  // First pass
#pragma omp parallel for schedule(static)
  for (int i = 0; i < static_cast<int>(height); ++i) {
    for (int j = 0; j < static_cast<int>(width); ++j) {
      if (source[i][j] == 0) {
        std::set<int> neighboringLabels;
        if (i > 0 && labels[(i - 1) * width + j] != -1) neighboringLabels.insert(labels[(i - 1) * width + j]);
        if (j > 0 && labels[i * width + (j - 1)] != -1) neighboringLabels.insert(labels[i * width + (j - 1)]);

        if (neighboringLabels.empty()) {
#pragma omp critical
          {
            labels[i * width + j] = nextLabel;
            equivalence[nextLabel].insert(nextLabel);
            ++nextLabel;
          }
        } else {
          int smallestLabel = *neighboringLabels.begin();
          labels[i * width + j] = smallestLabel;
          for (auto label : neighboringLabels) {
#pragma omp critical
            {
              equivalence[label].insert(smallestLabel);
              equivalence[smallestLabel].insert(label);
            }
          }
        }
      }
    }
  }

  // Resolve equivalences
#pragma omp parallel for schedule(static)
  for (int i = 0; i < static_cast<int>(equivalence.size()); ++i) {
    if (!equivalence[i].empty()) {
      int smallestLabel = *equivalence[i].begin();
#pragma omp parallel for schedule(static)
      for (int j = 0; j < static_cast<int>(width * height); ++j) {
        if (static_cast<int>(labels[j]) == i) {
          labels[j] = smallestLabel;
        }
      }
    }
  }

  resolve_labels(labels);

  // Second pass
#pragma omp parallel for schedule(static)
  for (int i = 0; i < static_cast<int>(height); ++i) {
    for (int j = 0; j < static_cast<int>(width); ++j) {
      if (source[i][j] == 0) {
        destination[i][j] = labels[i * width + j];
      }
    }
  }

  return true;
}

bool ImageMarkingOmp::post_processing() {
  internal_order_test();
  for (uint32_t i = 0; i < height; ++i)
    for (uint32_t j = 0; j < width; ++j)
      reinterpret_cast<uint32_t *>(taskData->outputs[0])[i * width + j] = destination[i][j];
  return true;
}

void ImageMarkingOmp::resolve_labels(std::vector<int> &labels) {
  std::unordered_map<int, int> label_map;
  int new_label = 1;
  for (auto &label : labels) {
    if (label != -1) {
      if (label_map.find(label) == label_map.end()) {
        label_map[label] = new_label++;
      }
      label = label_map[label];
    }
  }
}
