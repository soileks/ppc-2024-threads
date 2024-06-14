// Copyright 2024 Makhinya Danil
#include <gtest/gtest.h>

#include <functional>
#include <vector>

#include "tbb/makhinya_d_hoare_sort_batcher_merge/include/hoare_sort_tbb.hpp"

class MakhinyaDTestSort : public ::testing::Test {
 public:
  HoareSortTBB testTaskSequential;
  const uint32_t COUNT = 128;

  MakhinyaDTestSort() : testTaskSequential{HoareSortTBB(std::make_shared<ppc::core::TaskData>())} {}

  void set_data(HoareSortTBB::vec_t* in_vec, HoareSortTBB::compare_t* in_comp = nullptr) {
    std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();

    taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in_vec));
    taskDataSeq->inputs_count.emplace_back(1);

    if (in_comp != nullptr) {
      taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in_comp));
      taskDataSeq->inputs_count.emplace_back(1);
    }

    taskDataSeq->outputs.emplace_back(nullptr);
    taskDataSeq->outputs_count.emplace_back(0);

    testTaskSequential.set_data(taskDataSeq);
  }

  bool run_test() {
    if (!testTaskSequential.validation()) {
      return false;
    }

    testTaskSequential.pre_processing();
    testTaskSequential.run();
    testTaskSequential.post_processing();
    bool isSorted = testTaskSequential.check_order();
    return isSorted;
  }
};

TEST_F(MakhinyaDTestSort, Reverse_Vec) {
  HoareSortTBB::vec_t in_vec(COUNT);

  srand(time(nullptr));
  for (uint32_t i = 0; i < COUNT; ++i) {
    in_vec[i] = static_cast<HoareSortTBB::sortable_type>(COUNT - i);
  }

  set_data(&in_vec);
  EXPECT_TRUE(run_test());
}

TEST_F(MakhinyaDTestSort, Sorted_Vec) {
  HoareSortTBB::vec_t in_vec(COUNT);

  srand(time(nullptr));
  for (uint32_t i = 0; i < COUNT; ++i) {
    in_vec[i] = static_cast<HoareSortTBB::sortable_type>(i);
  }

  set_data(&in_vec);
  EXPECT_TRUE(run_test());
}

TEST_F(MakhinyaDTestSort, EQ_Vec) {
  HoareSortTBB::vec_t in_vec(COUNT);

  srand(time(nullptr));
  for (uint32_t i = 0; i < COUNT; ++i) {
    in_vec[i] = static_cast<HoareSortTBB::sortable_type>(COUNT);
  }

  set_data(&in_vec);
  EXPECT_TRUE(run_test());
}

TEST_F(MakhinyaDTestSort, Random_Vec) {
  HoareSortTBB::vec_t in_vec(COUNT);

  srand(time(nullptr));
  for (HoareSortTBB::sortable_type& x : in_vec) {
    x = static_cast<HoareSortTBB::sortable_type>(rand());
  }

  set_data(&in_vec);
  EXPECT_TRUE(run_test());
}

TEST_F(MakhinyaDTestSort, Random_Vec_With_Comp) {
  HoareSortTBB::vec_t in_vec(COUNT);

  HoareSortTBB::compare_t in_comp = [](const HoareSortTBB::sortable_type& a,
                                       const HoareSortTBB::sortable_type& b) -> bool {
    if ((a & 1) != (b & 1)) return (a & 1) < (b & 1);
    return a > b;
  };

  srand(time(nullptr));
  for (HoareSortTBB::sortable_type& x : in_vec) {
    x = static_cast<HoareSortTBB::sortable_type>(rand());
  }

  set_data(&in_vec, &in_comp);
  EXPECT_TRUE(run_test());
}
