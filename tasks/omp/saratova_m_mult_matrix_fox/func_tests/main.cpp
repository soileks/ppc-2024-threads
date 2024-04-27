// Copyright 2024 Saratova Marina
#include <gtest/gtest.h>
#include <omp.h>

#include <iostream>
#include <vector>

#include "omp/saratova_m_mult_matrix_fox/include/ops_seq.hpp"

using namespace saratova_omp;

TEST(Saratova_M_Mult_Matrix_Fox, Validation_Test) {
  size_t n = 2;
  std::vector<double> seq_in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> seq_in2{1.0, 0.0, 0.0, 1.0, 2.0};
  std::vector<double> par_in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> par_in2{1.0, 0.0, 0.0, 1.0, 2.0};
  std::vector<double> out_seq(n * n);
  std::vector<double> out_par(n * n);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in1.data()));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in2.data()));
  taskDataSeq->inputs_count.emplace_back(seq_in1.size());
  taskDataSeq->inputs_count.emplace_back(seq_in2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in1.data()));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in2.data()));
  taskDataPar->inputs_count.emplace_back(par_in1.size());
  taskDataPar->inputs_count.emplace_back(par_in2.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  SaratovaTaskSequential saratovaTaskSequential(taskDataSeq);
  SaratovaTaskOmp saratovaTaskOmp(taskDataPar);
  ASSERT_EQ(saratovaTaskSequential.validation(), false);
  ASSERT_EQ(saratovaTaskOmp.validation(), false);
}

TEST(Saratova_M_Mult_Matrix_Fox, Identity_Mult_On_Another_Matrix) {
  size_t n = 2;
  std::vector<double> seq_in1{0.0, 1.0, 1.0, 0.0};
  std::vector<double> seq_in2{24.0, 45.0, -31.0, 10.0};
  std::vector<double> par_in1{0.0, 1.0, 1.0, 0.0};
  std::vector<double> par_in2{24.0, 45.0, -31.0, 10.0};
  std::vector<double> out_seq(n * n);
  std::vector<double> out_par(n * n);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in1.data()));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in2.data()));
  taskDataSeq->inputs_count.emplace_back(seq_in1.size());
  taskDataSeq->inputs_count.emplace_back(seq_in2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  SaratovaTaskSequential saratovaTaskSequential(taskDataSeq);
  ASSERT_EQ(saratovaTaskSequential.validation(), true);
  saratovaTaskSequential.pre_processing();
  saratovaTaskSequential.run();
  saratovaTaskSequential.post_processing();

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in1.data()));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in2.data()));
  taskDataPar->inputs_count.emplace_back(par_in1.size());
  taskDataPar->inputs_count.emplace_back(par_in2.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  SaratovaTaskOmp saratovaTaskOmp(taskDataPar);
  ASSERT_EQ(saratovaTaskOmp.validation(), true);
  saratovaTaskOmp.pre_processing();
  saratovaTaskOmp.run();
  saratovaTaskOmp.post_processing();

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(out_seq[i * n + j], seq_in2[((n - i - 1)) * n + j]);
    }
  }
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(out_par[i * n + j], par_in2[((n - i - 1)) * n + j]);
    }
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, Identity_Mult_On_Matrix) {
  size_t n = 2;
  std::vector<double> seq_in1{1.0, 0.0, 0.0, 1.0};
  std::vector<double> seq_in2{24.0, 45.0, -31.0, 10.0};
  std::vector<double> par_in1{1.0, 0.0, 0.0, 1.0};
  std::vector<double> par_in2{24.0, 45.0, -31.0, 10.0};
  std::vector<double> out_seq(n * n);
  std::vector<double> out_par(n * n);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in1.data()));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in2.data()));
  taskDataSeq->inputs_count.emplace_back(seq_in1.size());
  taskDataSeq->inputs_count.emplace_back(seq_in2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  SaratovaTaskSequential saratovaTaskSequential(taskDataSeq);
  ASSERT_EQ(saratovaTaskSequential.validation(), true);
  saratovaTaskSequential.pre_processing();
  saratovaTaskSequential.run();
  saratovaTaskSequential.post_processing();

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in1.data()));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in2.data()));
  taskDataPar->inputs_count.emplace_back(par_in1.size());
  taskDataPar->inputs_count.emplace_back(par_in2.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  SaratovaTaskOmp saratovaTaskOmp(taskDataPar);
  ASSERT_EQ(saratovaTaskOmp.validation(), true);
  saratovaTaskOmp.pre_processing();
  saratovaTaskOmp.run();
  saratovaTaskOmp.post_processing();

  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(out_seq[i], seq_in2[i]);
  }
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(out_par[i], par_in2[i]);
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, Matrix_Mult_On_Identity) {
  size_t n = 2;
  std::vector<double> seq_in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> seq_in2{1.0, 0.0, 0.0, 1.0};
  std::vector<double> par_in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> par_in2{1.0, 0.0, 0.0, 1.0};
  std::vector<double> out_seq(n * n);
  std::vector<double> out_par(n * n);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in1.data()));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in2.data()));
  taskDataSeq->inputs_count.emplace_back(seq_in1.size());
  taskDataSeq->inputs_count.emplace_back(seq_in2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  SaratovaTaskSequential saratovaTaskSequential(taskDataSeq);
  ASSERT_EQ(saratovaTaskSequential.validation(), true);
  saratovaTaskSequential.pre_processing();
  saratovaTaskSequential.run();
  saratovaTaskSequential.post_processing();

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in1.data()));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in2.data()));
  taskDataPar->inputs_count.emplace_back(par_in1.size());
  taskDataPar->inputs_count.emplace_back(par_in2.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  SaratovaTaskOmp saratovaTaskOmp(taskDataPar);
  ASSERT_EQ(saratovaTaskOmp.validation(), true);
  saratovaTaskOmp.pre_processing();
  saratovaTaskOmp.run();
  saratovaTaskOmp.post_processing();

  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(out_seq[i], seq_in1[i]);
  }
  for (size_t i = 0; i < n * n; i++) {
    EXPECT_DOUBLE_EQ(out_par[i], par_in1[i]);
  }
}

TEST(Saratova_M_Mult_Matrix_Fox, Matrix_Mult_On_Another_Identity) {
  size_t n = 2;
  std::vector<double> seq_in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> seq_in2{0.0, 1.0, 1.0, 0.0};
  std::vector<double> par_in1{24.0, 45.0, -31.0, 10.0};
  std::vector<double> par_in2{0.0, 1.0, 1.0, 0.0};
  std::vector<double> out_seq(n * n);
  std::vector<double> out_par(n * n);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in1.data()));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(seq_in2.data()));
  taskDataSeq->inputs_count.emplace_back(seq_in1.size());
  taskDataSeq->inputs_count.emplace_back(seq_in2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(out_seq.size());

  SaratovaTaskSequential saratovaTaskSequential(taskDataSeq);
  ASSERT_EQ(saratovaTaskSequential.validation(), true);
  saratovaTaskSequential.pre_processing();
  saratovaTaskSequential.run();
  saratovaTaskSequential.post_processing();

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in1.data()));
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(par_in2.data()));
  taskDataPar->inputs_count.emplace_back(par_in1.size());
  taskDataPar->inputs_count.emplace_back(par_in2.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(out_par.size());

  SaratovaTaskOmp saratovaTaskOmp(taskDataPar);
  ASSERT_EQ(saratovaTaskOmp.validation(), true);
  saratovaTaskOmp.pre_processing();
  saratovaTaskOmp.run();
  saratovaTaskOmp.post_processing();

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(out_seq[i * n + j], seq_in1[i * n + (n - j - 1)]);
    }
  }
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      EXPECT_DOUBLE_EQ(out_par[i * n + j], par_in1[i * n + (n - j - 1)]);
    }
  }
}