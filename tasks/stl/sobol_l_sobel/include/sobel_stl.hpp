// Copyright 2024 Sobol Liubov
#ifndef SOBEL_STL_HPP
#define SOBEL_STL_HPP

#include <thread>
#include <vector>

#include "core/task/include/task.hpp"

namespace sobol {
struct RGB {
  uint8_t r, g, b;
};

class Sobel_stl : public ppc::core::Task {
 public:
  explicit Sobel_stl(std::shared_ptr<ppc::core::TaskData> taskData_, int w_, int h_)
      : Task(std::move(taskData_)), num_threads(std::thread::hardware_concurrency()), width(w_), height(h_) {}
  bool validation() override;
  bool pre_processing() override;
  bool run() override;
  bool post_processing() override;

 private:
  void sobel_thread(int start, int end);
  std::vector<std::thread> threads;
  int num_threads;

  void process_pixel(int i, int j);
  std::vector<RGB> input_;
  std::vector<RGB> res;
  int width, height;
};

std::vector<RGB> getRandomRGBPicture(int w, int h);
}  // namespace sobol

#endif  // SOBEL_STL_HPP
