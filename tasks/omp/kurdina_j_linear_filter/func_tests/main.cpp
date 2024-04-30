// Copyright 2024 Kurdina Julia
#include <gtest/gtest.h>

#include <vector>

#include "omp/kurdina_j_linear_filter/include/ops_omp.hpp"

TEST(kurdina_j_linear_filter_omp, Image_1) {
  int n = 3;
  int m = 3;
  std::vector<uint8_t> gauss_kernel = {1, 1, 1, 1, 9, 1, 1, 1, 1};

  // Create data
  std::vector<uint8_t> in = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<int> size_m = {n, m};
  std::vector<uint8_t> out(n * m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataSeq->inputs_count.emplace_back(size_m.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataSeq->inputs_count.emplace_back(gauss_kernel.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  HorizontalSplitOMP_seq horizontalsplit_seq(taskDataSeq);
  ASSERT_EQ(horizontalsplit_seq.validation(), true);
  horizontalsplit_seq.pre_processing();
  horizontalsplit_seq.run();
  horizontalsplit_seq.post_processing();
  
  // Create data
  std::vector<uint8_t> out_parallel(n * m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataPar->inputs_count.emplace_back(size_m.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataPar->inputs_count.emplace_back(gauss_kernel.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_parallel.data()));
  taskDataPar->outputs_count.emplace_back(out_parallel.size());

  // Create Task
  HorizontalSplitOMP_parallel horizontalsplit_parallel(taskDataPar);
  ASSERT_EQ(horizontalsplit_parallel.validation(), true);
  horizontalsplit_parallel.pre_processing();
  horizontalsplit_parallel.run();
  horizontalsplit_parallel.post_processing();

  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(out[i], out_parallel[i]);
  }
}

TEST(kurdina_j_linear_filter_omp, Image_2) {
  int n = 3;
  int m = 3;
  std::vector<uint8_t> gauss_kernel = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  // Create data
  std::vector<uint8_t> in = {255, 255, 255, 255, 255, 255, 255, 255, 255};
  std::vector<int> size_m = {n, m};
  std::vector<uint8_t> out(n * m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataSeq->inputs_count.emplace_back(size_m.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataSeq->inputs_count.emplace_back(gauss_kernel.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  HorizontalSplitOMP_seq horizontalsplit_seq(taskDataSeq);
  ASSERT_EQ(horizontalsplit_seq.validation(), true);
  horizontalsplit_seq.pre_processing();
  horizontalsplit_seq.run();
  horizontalsplit_seq.post_processing();

  // Create data
  std::vector<uint8_t> out_parallel(n * m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataPar->inputs_count.emplace_back(size_m.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataPar->inputs_count.emplace_back(gauss_kernel.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_parallel.data()));
  taskDataPar->outputs_count.emplace_back(out_parallel.size());

  // Create Task
  HorizontalSplitOMP_parallel horizontalsplit_parallel(taskDataPar);
  ASSERT_EQ(horizontalsplit_parallel.validation(), true);
  horizontalsplit_parallel.pre_processing();
  horizontalsplit_parallel.run();
  horizontalsplit_parallel.post_processing();

  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(out[i], out_parallel[i]);
  }
}

TEST(kurdina_j_linear_filter_omp, Image_3) {
  int n = 3;
  int m = 3;
  std::vector<uint8_t> gauss_kernel = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  // Create data
  std::vector<uint8_t> in = {1, 0, 2, 0, 3, 0, 4, 0, 5};
  std::vector<int> size_m = {n, m};
  std::vector<uint8_t> out(n * m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataSeq->inputs_count.emplace_back(size_m.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataSeq->inputs_count.emplace_back(gauss_kernel.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  HorizontalSplitOMP_seq horizontalsplit_seq(taskDataSeq);
  ASSERT_EQ(horizontalsplit_seq.validation(), true);
  horizontalsplit_seq.pre_processing();
  horizontalsplit_seq.run();
  horizontalsplit_seq.post_processing();

  // Create data
  std::vector<uint8_t> out_parallel(n * m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataPar->inputs_count.emplace_back(size_m.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataPar->inputs_count.emplace_back(gauss_kernel.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_parallel.data()));
  taskDataPar->outputs_count.emplace_back(out_parallel.size());

  // Create Task
  HorizontalSplitOMP_parallel horizontalsplit_parallel(taskDataPar);
  ASSERT_EQ(horizontalsplit_parallel.validation(), true);
  horizontalsplit_parallel.pre_processing();
  horizontalsplit_parallel.run();
  horizontalsplit_parallel.post_processing();

  for (int i = 0; i < n * m; i++) {
    ASSERT_EQ(out[i], out_parallel[i]);
  }
}

TEST(kurdina_j_linear_filter_omp, Check_validation_1) {
  int n = 3;
  int m = 3;
  std::vector<uint8_t> gauss_kernel(20);

  // Create data
  std::vector<uint8_t> in = {1, 0, 1, 0, 1, 0, 1, 0, 1};
  std::vector<int> size_m = {n, m};
  std::vector<uint8_t> out_parallel(n * m);
  std::vector<uint8_t> result = {5, 13, 31, 26, 78, 155, 134, 255, 255};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataPar->inputs_count.emplace_back(size_m.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataPar->inputs_count.emplace_back(gauss_kernel.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_parallel.data()));
  taskDataPar->outputs_count.emplace_back(out_parallel.size());

  // Create Task
  HorizontalSplitOMP_parallel horizontalsplit_parallel(taskDataPar);
  ASSERT_EQ(horizontalsplit_parallel.validation(), false);
}

TEST(kurdina_j_linear_filter_omp, Check_validation_2) {
  int n = 3;
  int m = 3;
  std::vector<uint8_t> gauss_kernel = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  // Create data
  std::vector<uint8_t> in = {1, 0, 1, 0, 1, 0, 1, 0, 1};
  std::vector<int> size_m = {n, m};
  std::vector<uint8_t> out_parallel(n * m * 100);
  std::vector<uint8_t> result = {5, 13, 31, 26, 78, 155, 134, 255, 255};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_m.data()));
  taskDataPar->inputs_count.emplace_back(size_m.size());
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(gauss_kernel.data()));
  taskDataPar->inputs_count.emplace_back(gauss_kernel.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_parallel.data()));
  taskDataPar->outputs_count.emplace_back(out_parallel.size());

  // Create Task
  HorizontalSplitOMP_parallel horizontalsplit_parallel(taskDataPar);
  ASSERT_EQ(horizontalsplit_parallel.validation(), false);
}
