// Copyright 2024 Sobol Liubov
#ifndef SOBEL_OMP_HPP
#define SOBEL_OMP_HPP

#include <vector>

#include "core/task/include/task.hpp"

namespace sobol {
struct RGB {
  uint8_t r, g, b;
};

class Sobel_omp : public ppc::core::Task {
 public:
  explicit Sobel_omp(std::shared_ptr<ppc::core::TaskData> taskData_, int w_, int h_)
      : Task(std::move(taskData_)), width(w_), height(h_) {}
  bool validation() override;
  bool pre_processing() override;
  bool run() override;
  bool post_processing() override;

 private:
  void process_pixel(int i, int j);
  std::vector<RGB> input_;
  std::vector<RGB> res;
  int width, height;
};

std::vector<RGB> getRandomRGBPicture(int w, int h);
}  // namespace sobol

#endif  // SOBEL_OMP_HPP
