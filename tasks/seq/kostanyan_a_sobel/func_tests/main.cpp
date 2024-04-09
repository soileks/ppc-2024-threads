// Copyright 2023 Kostanyan Arsen
#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include "seq/kostanyan_a_sobel/include/ops_seq.hpp"

TEST(kostanyan_a_sobel_seq, Test_EdgeDetection) {
  const int n = 10;
  const int m = 10;
  const uint8_t min = 0;
  const uint8_t max = 255;

  // Create data
  std::vector<int> in(2);
  in[0] = n;
  in[1] = m;
  std::vector<uint8_t> pict = kostanyan_getRandomPicture(n, m, min, max);
  std::vector<uint8_t> out(n * m, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(pict.data());
  taskDataSeq->inputs_count.emplace_back(pict.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  kostanyan_EdgeDetectionSequential kostanyan_EdgeDetectionSequential(taskDataSeq);
  ASSERT_EQ(kostanyan_EdgeDetectionSequential.validation(), true);
  kostanyan_EdgeDetectionSequential.pre_processing();
  kostanyan_EdgeDetectionSequential.run();
  kostanyan_EdgeDetectionSequential.post_processing();

  // Check result
  for (int i = 1; i < n - 1; i++) {
    for (int j = 1; j < m - 1; j++) {
      int gx = -pict[(i - 1) * m + j - 1] + pict[(i - 1) * m + j + 1] - 2 * pict[i * m + j - 1] +
               2 * pict[i * m + j + 1] - pict[(i + 1) * m + j - 1] + pict[(i + 1) * m + j + 1];
      int gy = pict[(i - 1) * m + j - 1] + 2 * pict[(i - 1) * m + j] + pict[(i - 1) * m + j + 1] -
               pict[(i + 1) * m + j - 1] - 2 * pict[(i + 1) * m + j] - pict[(i + 1) * m + j + 1];
      uint8_t expected = sqrt(gx * gx + gy * gy);
      ASSERT_EQ(expected, out[i * m + j]);
    }
  }
}

TEST(kostanyan_a_sobel_seq, Test_Empty_Image) {
  const int n = 0;
  const int m = 0;

  // Create data
  std::vector<int> in(2);
  in[0] = n;
  in[1] = m;
  std::vector<uint8_t> pict;
  std::vector<uint8_t> out;

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(pict.data());
  taskDataSeq->inputs_count.emplace_back(pict.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  kostanyan_EdgeDetectionSequential kostanyan_EdgeDetectionSequential(taskDataSeq);
  ASSERT_EQ(kostanyan_EdgeDetectionSequential.validation(), true);
  kostanyan_EdgeDetectionSequential.pre_processing();
  ASSERT_EQ(kostanyan_EdgeDetectionSequential.run(), false);
}

TEST(kostanyan_a_sobel_seq, Test_Single_Pixel_Image) {
  const int n = 1;
  const int m = 1;
  const uint8_t min = 0;
  const uint8_t max = 255;

  // Create data
  std::vector<int> in(2);
  in[0] = n;
  in[1] = m;
  std::vector<uint8_t> pict = kostanyan_getRandomPicture(n, m, min, max);
  std::vector<uint8_t> out(n * m, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(pict.data());
  taskDataSeq->inputs_count.emplace_back(pict.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  kostanyan_EdgeDetectionSequential kostanyan_EdgeDetectionSequential(taskDataSeq);
  ASSERT_EQ(kostanyan_EdgeDetectionSequential.validation(), true);
  kostanyan_EdgeDetectionSequential.pre_processing();
  kostanyan_EdgeDetectionSequential.run();
  kostanyan_EdgeDetectionSequential.post_processing();
  ASSERT_EQ(pict[0], out[0]);
}

TEST(kostanyan_a_sobel_seq, Test_All_White_Image) {
  const int n = 10;
  const int m = 10;
  const uint8_t white = 255;

  // Create data
  std::vector<int> in(2);
  in[0] = n;
  in[1] = m;
  std::vector<uint8_t> pict(n * m, white);
  std::vector<uint8_t> out(n * m, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(pict.data());
  taskDataSeq->inputs_count.emplace_back(pict.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  kostanyan_EdgeDetectionSequential kostanyan_EdgeDetectionSequential(taskDataSeq);
  ASSERT_EQ(kostanyan_EdgeDetectionSequential.validation(), true);
  kostanyan_EdgeDetectionSequential.pre_processing();
  kostanyan_EdgeDetectionSequential.run();
  kostanyan_EdgeDetectionSequential.post_processing();

  for (int i = 1; i < n - 1; i++) {
    for (int j = 1; j < m - 1; j++) {
      ASSERT_EQ(0, out[i * m + j]);
    }
  }
}

TEST(kostanyan_a_sobel_seq, Test_Large_Image) {
  const int n = 100;
  const int m = 100;
  const uint8_t min = 0;
  const uint8_t max = 255;

  // Create data
  std::vector<int> in(2);
  in[0] = n;
  in[1] = m;
  std::vector<uint8_t> pict = kostanyan_getRandomPicture(n, m, min, max);
  std::vector<uint8_t> out(n * m, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->inputs.emplace_back(pict.data());
  taskDataSeq->inputs_count.emplace_back(pict.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  kostanyan_EdgeDetectionSequential kostanyan_EdgeDetectionSequential(taskDataSeq);
  ASSERT_EQ(kostanyan_EdgeDetectionSequential.validation(), true);
  kostanyan_EdgeDetectionSequential.pre_processing();
  kostanyan_EdgeDetectionSequential.run();
  kostanyan_EdgeDetectionSequential.post_processing();

  for (int i = 1; i < n - 1; i++) {
    for (int j = 1; j < m - 1; j++) {
      int gx = -pict[(i - 1) * m + j - 1] + pict[(i - 1) * m + j + 1] - 2 * pict[i * m + j - 1] +
               2 * pict[i * m + j + 1] - pict[(i + 1) * m + j - 1] + pict[(i + 1) * m + j + 1];
      int gy = pict[(i - 1) * m + j - 1] + 2 * pict[(i - 1) * m + j] + pict[(i - 1) * m + j + 1] -
               pict[(i + 1) * m + j - 1] - 2 * pict[(i + 1) * m + j] - pict[(i + 1) * m + j + 1];
      uint8_t expected = sqrt(gx * gx + gy * gy);
      ASSERT_EQ(expected, out[i * m + j]);
    }
  }
}
