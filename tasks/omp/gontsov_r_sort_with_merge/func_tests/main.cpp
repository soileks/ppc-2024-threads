// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <vector>

#include "omp/gontsov_r_sort_with_merge/include/ops_omp.hpp"

TEST(Gontsov_R_Radix_Sort, can_create_task_succseccfully) {
  std::vector<int> in = {45, 12, 89, 33, 27, 150, 2, 58, 9, 66};
  std::vector<int> exp = {2, 9, 12, 27, 33, 45, 58, 66, 89, 150};

  // Create data
  std::vector<int> out2(exp.size(), 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(in.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out2.data()));
  taskDataPar->outputs_count.emplace_back(out2.size());

  // Create Task
  RadixSortOMPParallel radixSortOMPParallel(taskDataPar);
  ASSERT_EQ(radixSortOMPParallel.validation(), true);
  ASSERT_TRUE(radixSortOMPParallel.pre_processing());
  ASSERT_TRUE(radixSortOMPParallel.run());
  ASSERT_TRUE(radixSortOMPParallel.post_processing());

  for (size_t i = 0; i < out2.size(); i++) {
    ASSERT_EQ(out2[i], exp[i]);
  }
}

//int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
