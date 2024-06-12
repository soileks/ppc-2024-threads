#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <vector>

#include "tbb/Vasilev_ivan_shell_sort/include/ops_tbb.hpp"

class PerformanceTest : public ::testing::Test {
 protected:
  void runTest(int size) {
    std::vector<int> arr = VasilevTaskTbb::generate_random_vector(size, 0, size);
    std::vector<int> rez(size);

    // Create TaskData
    std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
    taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(&arr));
    taskData->inputs_count.emplace_back(1);
    taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(&rez));
    taskData->outputs_count.emplace_back(1);

    // Create Task
    VasilevTaskTbb testTask(taskData);
    ASSERT_EQ(testTask.validation(), true);

    auto start = std::chrono::high_resolution_clock::now();

    testTask.pre_processing();
    testTask.run();
    testTask.post_processing();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Sorting " << size << " elements took " << duration.count() << " seconds." << std::endl;

    ASSERT_TRUE(VasilevTaskTbb::R_sorted(rez));
  }
};

TEST_F(PerformanceTest, Sort_1K) { runTest(1000); }

TEST_F(PerformanceTest, Sort_10K) { runTest(10000); }
