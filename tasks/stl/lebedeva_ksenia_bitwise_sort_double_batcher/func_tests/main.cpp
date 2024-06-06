// Copyright 2024 Lebedeva Ksenia

#include <gtest/gtest.h>

#include "stl/lebedeva_ksenia_bitwise_sort_double_batcher/include/ops_stl.hpp"

TEST(lebedeva_ksenia_bitwise_sort_double_batcher, arr) {
  // Create data
  std::vector<double> arr = {3.587, 7.924, 0.315, 5.871, 2.108, 9.736, 4.269, 6.482, 1.593, 8.176};
  // Create data
  std::vector<double> out_seq(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataSeq = std::make_shared<ppc::core::TaskData>();
  RSDBDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataSeq->inputs_count.emplace_back(arr.size());
  RSDBDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  RSDBDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherSequential testRDSBSequential(RSDBDataSeq);
  ASSERT_EQ(testRDSBSequential.validation(), true);
  testRDSBSequential.pre_processing();
  testRDSBSequential.run();
  testRDSBSequential.post_processing();

  // Create data
  std::vector<double> out_par(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataStl = std::make_shared<ppc::core::TaskData>();
  RSDBDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataStl->inputs_count.emplace_back(arr.size());
  RSDBDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  RSDBDataStl->outputs_count.emplace_back(out_par.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherStlParallel testRDSBstlParallel(RSDBDataStl);
  ASSERT_EQ(testRDSBstlParallel.validation(), true);
  testRDSBstlParallel.pre_processing();
  testRDSBstlParallel.run();
  testRDSBstlParallel.post_processing();

  for (size_t i = 0; i < out_seq.size(); ++i) {
    ASSERT_EQ(out_seq[i], out_par[i]);
  }
}

TEST(lebedeva_ksenia_bitwise_sort_double_batcher, test_arr_2) {
  // Create data
  std::vector<double> arr = {1.345, 5.672, 8.943, 3.219, 6.785, 9.234, 2.109, 4.567, 7.890, 0.123, 2.456, 6.789, 3.456,
                             8.901, 1.234, 9.876, 5.432, 0.987, 4.321, 7.654, 2.109, 8.765, 3.098, 9.876, 0.543, 6.789,
                             4.321, 1.234, 7.890, 5.432, 3.210, 2.345, 8.976, 6.543, 9.012, 0.987, 4.321, 7.654, 5.678,
                             3.456, 1.234, 9.876, 2.109, 8.765, 0.543, 6.789, 4.321, 7.890, 3.251, 7.539};
  // Create data
  std::vector<double> out_seq(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataSeq = std::make_shared<ppc::core::TaskData>();
  RSDBDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataSeq->inputs_count.emplace_back(arr.size());
  RSDBDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  RSDBDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherSequential testRDSBSequential(RSDBDataSeq);
  ASSERT_EQ(testRDSBSequential.validation(), true);
  testRDSBSequential.pre_processing();
  testRDSBSequential.run();
  testRDSBSequential.post_processing();

  // Create data
  std::vector<double> out_par(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataStl = std::make_shared<ppc::core::TaskData>();
  RSDBDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataStl->inputs_count.emplace_back(arr.size());
  RSDBDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  RSDBDataStl->outputs_count.emplace_back(out_par.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherStlParallel testRDSBstlParallel(RSDBDataStl);
  ASSERT_EQ(testRDSBstlParallel.validation(), true);
  testRDSBstlParallel.pre_processing();
  testRDSBstlParallel.run();
  testRDSBstlParallel.post_processing();

  for (size_t i = 0; i < out_seq.size(); ++i) {
    ASSERT_EQ(out_seq[i], out_par[i]);
  }
}
TEST(lebedeva_ksenia_bitwise_sort_double_batcher, test_empty) {
  // Create data
  std::vector<double> arr = {};
  // Create data
  std::vector<double> out_seq(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataSeq = std::make_shared<ppc::core::TaskData>();
  RSDBDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataSeq->inputs_count.emplace_back(arr.size());
  RSDBDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  RSDBDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherSequential testRDSBSequential(RSDBDataSeq);
  ASSERT_EQ(testRDSBSequential.validation(), true);
  testRDSBSequential.pre_processing();
  testRDSBSequential.run();
  testRDSBSequential.post_processing();

  // Create data
  std::vector<double> out_par(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataStl = std::make_shared<ppc::core::TaskData>();
  RSDBDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataStl->inputs_count.emplace_back(arr.size());
  RSDBDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  RSDBDataStl->outputs_count.emplace_back(out_par.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherStlParallel testRDSBstlParallel(RSDBDataStl);
  ASSERT_EQ(testRDSBstlParallel.validation(), true);
  testRDSBstlParallel.pre_processing();
  testRDSBstlParallel.run();
  testRDSBstlParallel.post_processing();

  for (size_t i = 0; i < out_seq.size(); ++i) {
    ASSERT_EQ(out_seq[i], out_par[i]);
  }
}
TEST(lebedeva_ksenia_bitwise_sort_double_batcher, test_one_number) {
  // Create data
  std::vector<double> arr = {1};
  // Create data
  std::vector<double> out_seq(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataSeq = std::make_shared<ppc::core::TaskData>();
  RSDBDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataSeq->inputs_count.emplace_back(arr.size());
  RSDBDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  RSDBDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherSequential testRDSBSequential(RSDBDataSeq);
  ASSERT_EQ(testRDSBSequential.validation(), true);
  testRDSBSequential.pre_processing();
  testRDSBSequential.run();
  testRDSBSequential.post_processing();

  // Create data
  std::vector<double> out_par(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataStl = std::make_shared<ppc::core::TaskData>();
  RSDBDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataStl->inputs_count.emplace_back(arr.size());
  RSDBDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  RSDBDataStl->outputs_count.emplace_back(out_par.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherStlParallel testRDSBstlParallel(RSDBDataStl);
  ASSERT_EQ(testRDSBstlParallel.validation(), true);
  testRDSBstlParallel.pre_processing();
  testRDSBstlParallel.run();
  testRDSBstlParallel.post_processing();

  for (size_t i = 0; i < out_seq.size(); ++i) {
    ASSERT_EQ(out_seq[i], out_par[i]);
  }
}
TEST(lebedeva_ksenia_bitwise_sort_double_batcher, test_with_negative) {
  // Create data
  std::vector<double> arr = {-3.217, 5.678, -1.234, 8.901, -2.345, 6.789, 4.321,
                             -7.654, 2.109, -9.876, 0.543, 3.456,  -8.765};
  // Create data
  std::vector<double> out_seq(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataSeq = std::make_shared<ppc::core::TaskData>();
  RSDBDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataSeq->inputs_count.emplace_back(arr.size());
  RSDBDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  RSDBDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherSequential testRDSBSequential(RSDBDataSeq);
  ASSERT_EQ(testRDSBSequential.validation(), true);
  testRDSBSequential.pre_processing();
  testRDSBSequential.run();
  testRDSBSequential.post_processing();

  // Create data
  std::vector<double> out_par(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataStl = std::make_shared<ppc::core::TaskData>();
  RSDBDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataStl->inputs_count.emplace_back(arr.size());
  RSDBDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  RSDBDataStl->outputs_count.emplace_back(out_par.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherStlParallel testRDSBstlParallel(RSDBDataStl);
  ASSERT_EQ(testRDSBstlParallel.validation(), true);
  testRDSBstlParallel.pre_processing();
  testRDSBstlParallel.run();
  testRDSBstlParallel.post_processing();

  for (size_t i = 0; i < out_seq.size(); ++i) {
    ASSERT_EQ(out_seq[i], out_par[i]);
  }
}

TEST(lebedeva_ksenia_bitwise_sort_double_batcher, test_random) {
  // Create data
  std::vector<double> arr = LebedevaKStl::randomVector(1000, -1000000, 1000000);
  // Create data
  std::vector<double> out_seq(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataSeq = std::make_shared<ppc::core::TaskData>();
  RSDBDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataSeq->inputs_count.emplace_back(arr.size());
  RSDBDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  RSDBDataSeq->outputs_count.emplace_back(out_seq.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherSequential testRDSBSequential(RSDBDataSeq);
  ASSERT_EQ(testRDSBSequential.validation(), true);
  testRDSBSequential.pre_processing();
  testRDSBSequential.run();
  testRDSBSequential.post_processing();

  // Create data
  std::vector<double> out_par(arr.size());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> RSDBDataStl = std::make_shared<ppc::core::TaskData>();
  RSDBDataStl->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
  RSDBDataStl->inputs_count.emplace_back(arr.size());
  RSDBDataStl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  RSDBDataStl->outputs_count.emplace_back(out_par.size());

  // Create Task
  LebedevaKStl::RadixSortDoubleWithBatcherStlParallel testRDSBstlParallel(RSDBDataStl);
  ASSERT_EQ(testRDSBstlParallel.validation(), true);
  testRDSBstlParallel.pre_processing();
  testRDSBstlParallel.run();
  testRDSBstlParallel.post_processing();

  for (size_t i = 0; i < out_seq.size(); ++i) {
    ASSERT_EQ(out_seq[i], out_par[i]);
  }
}
