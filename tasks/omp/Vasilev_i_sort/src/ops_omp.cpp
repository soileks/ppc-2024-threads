// Copyright 2024 Vasilev Ivan
// #include <thread>

#include "omp/Vasilev_i_sort/include/ops_omp.hpp"

#include <algorithm>
// using namespace std::chrono_literals;

bool VasilevTaskOmp::pre_processing() {
  internal_order_test();
  // Init value for input and output
  input_ = *reinterpret_cast<std::vector<int>*>(taskData->inputs[0]);
  res = *reinterpret_cast<std::vector<int>*>(taskData->outputs[0]);
  return true;
}

bool VasilevTaskOmp::validation() {
  internal_order_test();
  // Check count elements of output
  // return taskData->inputs_count[0] == 1 && taskData->outputs_count[0] == 1;
  return true;
}

bool VasilevTaskOmp::run() {
  internal_order_test();
  shell_sort_batcher(input_, input_.size());
  return true;
}

bool VasilevTaskOmp::post_processing() {
  internal_order_test();
  // reinterpret_cast<int*>(taskData->outputs[0])[0] = res;
  reinterpret_cast<std::vector<int>*>(taskData->outputs[0])->operator=(input_);

  return true;
}
void VasilevTaskOmp::shell_sort_batcher(std::vector<int>& arr, int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
      bool sorted = false;
      while (!sorted) {
        sorted = true;

#pragma omp parallel for
      for (int i = 0; i < n - gap; i += 2) {
        if (arr[i] > arr[i + gap]) {
          std::swap(arr[i], arr[i + gap]);
          sorted = false;
        }
      }

#pragma omp parallel for
      for (int i = 1; i < n - gap; i += 2) {
        if (arr[i] > arr[i + gap]) {
          std::swap(arr[i], arr[i + gap]);
          sorted = false;
        }
      }
    }
  }
} 
// Функция для слияния отсортированных массивов
// void VasilevTaskOmp::merge(int l, int m, int r) {
//   int n1 = m - l + 1;
//   int n2 = r - m;

//   // создаем временные массивы
//   std::vector<int> L(n1);
//   std::vector<int> R(n2);

//   // копируем данные во временные массивы
//   for (int i = 0; i < n1; i++) {
//     L[i] = input_[l + i];
//   }
//   for (int j = 0; j < n2; j++) {
//     R[j] = input_[m + 1 + j];
//   }

//   // слияние временных массивов обратно в основной массив
//   int i = 0;
//   int j = 0;
//   int k = l;
//   while (i < n1 && j < n2) {
//     if (L[i] <= R[j]) {
//       input_[k] = L[i];
//       i++;
//     } else {
//       input_[k] = R[j];
//       j++;
//     }
//     k++;
//   }

//   // остаток элементов
//   while (i < n1) {
//     input_[k] = L[i];
//     i++;
//     k++;
//   }
//   while (j < n2) {
//     input_[k] = R[j];
//     j++;
//     k++;
//   }
// }

// Сортировка Шелла
// void VasilevTaskOmp::shellSort(std::vector<int>& arr, int n) {
//    for (int gap = n / 2; gap > 0; gap /= 2) {
//         #pragma omp parallel for
//         for (int i = gap; i < n; i += 1) {
//             int temp = arr[i];
//             int j;
//             for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
//                 arr[j] = arr[j - gap];
//             arr[j] = temp;
//         }
//     }
// }

// Четно-нечетное слияние Бэтчера
// void VasilevTaskOmp::batcherMerge(int l, int r) {
//   shellSort();
//   if (r > l) {
//     int m = l + (r - l) / 2;
//     for (int i = l; i <= m; i += 2) {
//       if (i + 1 <= m) {
//         if (input_[i] > input_[i + 1]) {
//           std::swap(input_[i], input_[i + 1]);
//         }
//       }
//     }
//     batcherMerge(l, m);
//     batcherMerge(m + 1, r);
//     merge(l, m, r);
//   }
// }
bool VasilevTaskOmp::R_sorted(std::vector<int> input) { return std::is_sorted(input.begin(), input.end()); }

std::vector<int> VasilevTaskOmp::generate_random_vector(int size, int min, int max) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine{rnd_device()};  // Generates random integers
  std::uniform_int_distribution<int> dist{min, max};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

  std::vector<int> vec(size);
  generate(begin(vec), end(vec), gen);

  return vec;
}
