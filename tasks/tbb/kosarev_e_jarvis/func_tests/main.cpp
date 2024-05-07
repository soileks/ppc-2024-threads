// Copyright 2024 Kosarev Egor
#include <gtest/gtest.h>



#include "tbb/kosarev_e_jarvis/include/ops_tbb.hpp"

TEST(Kosarev_e_jarvis_tbb, Test_hull_one) {
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> points = {{0, 3}, {1, 1}, {2, 2}, {4, 4},
                                                                {0, 0}, {1, 2}, {3, 1}, {3, 3}};
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskSequentialKosarevJarvisHull testTbbTaskSequential(taskDataSeq);
  ASSERT_EQ(testTbbTaskSequential.validation(), true);
  testTbbTaskSequential.pre_processing();
  testTbbTaskSequential.run();
  testTbbTaskSequential.post_processing();

  // Create data
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskTbbKosarevJarvisHull testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  testTbbTaskParallel.pre_processing();
  testTbbTaskParallel.run();
  testTbbTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_tbb, Test_hull_two) {
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> points = {
      {2, 5}, {7, 3}, {1, 1}, {-1, 4}, {-5, 4}, {-4, 1}, {-1, -2}, {2, -3}, {3, -1}, {2, 2}, {1, 5}, {-4, 5}, {-1, 0}};
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskSequentialKosarevJarvisHull testTbbTaskSequential(taskDataSeq);
  ASSERT_EQ(testTbbTaskSequential.validation(), true);
  testTbbTaskSequential.pre_processing();
  testTbbTaskSequential.run();
  testTbbTaskSequential.post_processing();

  // Create data
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskTbbKosarevJarvisHull testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  testTbbTaskParallel.pre_processing();
  testTbbTaskParallel.run();
  testTbbTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_tbb, Test_hull_three) {
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> points = {{2, 5}, {7, 3}, {1, 1}};
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskSequentialKosarevJarvisHull testTbbTaskSequential(taskDataSeq);
  ASSERT_EQ(testTbbTaskSequential.validation(), true);
  testTbbTaskSequential.pre_processing();
  testTbbTaskSequential.run();
  testTbbTaskSequential.post_processing();

  // Create data
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskTbbKosarevJarvisHull testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  testTbbTaskParallel.pre_processing();
  testTbbTaskParallel.run();
  testTbbTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_tbb, Test_hull_no_hull) {
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> points = {};
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskSequentialKosarevJarvisHull testTbbTaskSequential(taskDataSeq);
  ASSERT_EQ(testTbbTaskSequential.validation(), true);
  testTbbTaskSequential.pre_processing();
  testTbbTaskSequential.run();
  testTbbTaskSequential.post_processing();

  // Create data
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskTbbKosarevJarvisHull testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  testTbbTaskParallel.pre_processing();
  testTbbTaskParallel.run();
  testTbbTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}

TEST(Kosarev_e_jarvis_tbb, Test_hull_five) {
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> points = {{1, 4}, {1, 5}, {1, 6}, {1, 2}, {1, 3},
                                                                {2, 5}, {3, 4}, {0, 5}, {1, 7}};
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_seq(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataSeq->inputs_count.emplace_back(points.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_seq.data()));
  taskDataSeq->outputs_count.emplace_back(resHull_seq.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskSequentialKosarevJarvisHull testTbbTaskSequential(taskDataSeq);
  ASSERT_EQ(testTbbTaskSequential.validation(), true);
  testTbbTaskSequential.pre_processing();
  testTbbTaskSequential.run();
  testTbbTaskSequential.post_processing();

  // Create data
  std::vector<Kosarev_e_tbb_KosarevJarvisHull::Point> resHull_par(points.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskDataPar->inputs_count.emplace_back(points.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(resHull_par.data()));
  taskDataPar->outputs_count.emplace_back(resHull_par.size());

  // Create Task
  Kosarev_e_tbb_KosarevJarvisHull::TestTaskTbbKosarevJarvisHull testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  testTbbTaskParallel.pre_processing();
  testTbbTaskParallel.run();
  testTbbTaskParallel.post_processing();

  for (size_t i = 0; i < resHull_par.size(); ++i) {
    ASSERT_EQ(resHull_par[i], resHull_seq[i]);
  }
}