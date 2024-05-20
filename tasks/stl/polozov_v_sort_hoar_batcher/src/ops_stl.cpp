// Copyright 2024 Polozov Vladislav
#include "stl/polozov_v_sort_hoar_batcher/include/ops_stl.hpp"

bool SortHoarWithMergeBatcher::pre_processing() {
  internal_order_test();
  // Init value for input and output
  arr = std::vector<int>(taskData->inputs_count[0]);

  auto* temp_arr = reinterpret_cast<int*>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    arr[i] = temp_arr[i];
  }
  return true;
}

bool SortHoarWithMergeBatcher::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] == taskData->outputs_count[0];
}

bool SortHoarWithMergeBatcher::run() {
  internal_order_test();
  res = odd_even_merge_with_hoar(arr);
  return true;
}

bool SortHoarWithMergeBatcher::post_processing() {
  internal_order_test();
  std::copy(res.begin(), res.end(), reinterpret_cast<int*>(taskData->outputs[0]));
  return true;
}

int partition(std::vector<int>& a, int l, int r) {
  int pivot = a[r];
  int m = l;
  for (int i = l; i <= r; i++) {
    if (a[i] <= pivot) {
      std::swap(a[i], a[m]);
      m++;
    }
  }
  return m - 1;
}

void Hoar_sort(std::vector<int>& a, int l, int r) {
  if (r <= l) return;
  int m = partition(a, l, r);
  Hoar_sort(a, l, m - 1);
  Hoar_sort(a, m + 1, r);
}

std::vector<int> generate_data(int n, int mn, int mx) {
  std::random_device rnd;
  std::default_random_engine generate{rnd()};
  std::uniform_real_distribution<double> random(mn, mx);
  std::vector<int> ans(n);
  for (int i = 0; i < n; i++) {
    ans[i] = random(generate);
  }
  return ans;
}

inline void CompAndSwap(int& x, int& y) {
  if (x > y) std::swap(x, y);
}

void Calc(std::vector<int>& my_data, int k, int L, int R, int l) {
  for(int i = L; i <= R; i++){
    for (int j = 0;j < k; j++){
        CompAndSwap(my_data[l + 2 * k * i + j], my_data[l + 2 * k * i + j + k]);
    }
  }

}

std::vector<int> odd_even_merge_with_hoar(std::vector<int> my_data) {
  if (my_data.size() <= 8) {
    Hoar_sort(my_data, 0, my_data.size() - 1);
    std::vector<int> ans = my_data;
    sort(ans.begin(), ans.end());
    return ans;
  }
  int n = my_data.size();
  auto as1 = std::async([&] { Hoar_sort(my_data, 0 * n / 4, (1) * n / 4 - 1); });
  auto as2 = std::async([&] { Hoar_sort(my_data, 1 * n / 4, (2) * n / 4 - 1); });
  auto as3 = std::async([&] { Hoar_sort(my_data, 2 * n / 4, (3) * n / 4 - 1); });
  auto as4 = std::async([&] { Hoar_sort(my_data, 3 * n / 4, (4) * n / 4 - 1); });
  as1.wait();
  as2.wait();
  as3.wait();
  as4.wait();
  std::vector<int> sizes(num_max_thread);
  auto merge = [&](int l, int r) {
    int n = (r - l + 1);
    for (int i = 0; i < n / 2; i++) {
      CompAndSwap(my_data[l + i], my_data[r - i]);
    }
    for (int k = n / 2; k >= 2; k /= 2) {
      int each = (n / k) / (num_max_thread);
      sizes.assign(num_max_thread, each);
      sizes.back() += (n / k) % (num_max_thread);
      auto a1 = std::async([&] { Calc(my_data, k / 2, 0, sizes[0] - 1, l);});
      auto a2 = std::async([&] { Calc(my_data, k / 2, sizes[0], sizes[0] + sizes[1] - 1, l);});
      auto a3 = std::async([&] { Calc(my_data, k / 2, sizes[0] + sizes[1], sizes[0] + sizes[1] + sizes[2] - 1, l);});
      auto a4 = std::async([&] { Calc(my_data, k / 2, sizes[0] + sizes[1] + sizes[2], n/k - 1, l);});
      a1.wait();
      a2.wait();
      a3.wait();
      a4.wait();
    }
  };

  merge(0, n / 2 - 1);
  merge(n / 2, n - 1);
  merge(0, n - 1);
  return my_data;
}
