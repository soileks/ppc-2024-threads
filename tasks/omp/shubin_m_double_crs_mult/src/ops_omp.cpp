// Copyright 2024 Shubin Mikhail

#include "omp/shubin_m_double_crs_mult/include/ops_omp.hpp"

bool SparseMat_CRS_Mult_OMP::validation() {
  internal_order_test();

  size_t mat_in_1_col_c = reinterpret_cast<SparseMat_CRS*>(taskData->inputs[0])->col_c;
  size_t mat_in_2_row_c = reinterpret_cast<SparseMat_CRS*>(taskData->inputs[1])->row_c;

  return (mat_in_1_col_c == mat_in_2_row_c);
}

bool SparseMat_CRS_Mult_OMP::pre_processing() {
  internal_order_test();

  mat_in_1 = reinterpret_cast<SparseMat_CRS*>(taskData->inputs[0]);
  mat_in_2 = reinterpret_cast<SparseMat_CRS*>(taskData->inputs[1]);
  mat_out = reinterpret_cast<SparseMat_CRS*>(taskData->outputs[0]);

  return true;
}

bool SparseMat_CRS_Mult_OMP::run() {
  internal_order_test();

  std::vector<std::vector<double>> glob_row(mat_in_1->row_c);
  std::vector<std::vector<size_t>> glob_col_ind(mat_in_1->row_c);

#pragma omp parallel for default(none) shared(glob_row, glob_col_ind) schedule(static)
  for (int i = 0; i < mat_in_1->row_c; i++) {
    std::vector<double> loc_row(mat_out->col_c, 0.0);
    for (size_t j = mat_in_1->row_ind[i]; j < mat_in_1->row_ind[i + 1]; j++) {
      size_t col = mat_in_1->col_ind[j];
      double temp = mat_in_1->val[j];
      for (size_t k = mat_in_2->row_ind[col]; k < mat_in_2->row_ind[col + 1]; k++) {
        loc_row[mat_in_2->col_ind[k]] += temp * mat_in_2->val[k];
      }
    }

    for (size_t j = 0; j < mat_out->col_c; j++) {
      if (std::abs(loc_row[j]) > PRECISION) {
        glob_row[i].push_back(loc_row[j]);
        glob_col_ind[i].push_back(j);
      }
    }
  }

  for (size_t i = 0; i < mat_out->row_c; i++) {
    mat_out->row_ind.push_back(mat_out->val.size());
    for (size_t j = 0; j < glob_row[i].size(); j++) {
      mat_out->val.push_back(glob_row[i][j]);
      mat_out->col_ind.push_back(glob_col_ind[i][j]);
    }
  }
  mat_out->row_ind.push_back(mat_out->val.size());

  return true;
}

bool SparseMat_CRS_Mult_OMP::post_processing() {
  internal_order_test();

  return true;
}
