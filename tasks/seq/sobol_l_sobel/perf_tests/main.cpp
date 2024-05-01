// Copyright 2024 Sobol Liubov
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/sobol_l_sobel/include/sobel_seq.hpp"

TEST(sobol_l_sobel_seq, test_pipeline_run) {
  int width = 1000, height = 1000;

  // Создание тестовых данных
  std::vector<sobol::RGB> input = sobol::getRandomRGBPicture(width, height);
  std::vector<sobol::RGB> output(width * height);

  // Инициализация данных задачи
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(output.size() * sizeof(sobol::RGB)));

  // Создание задачи
  std::shared_ptr<sobol::Sobel_seq> sobel = std::make_shared<sobol::Sobel_seq>(taskData, width, height);

  // Создание атрибутов производительности
  std::shared_ptr<ppc::core::PerfAttr> perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] {
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  };

  // Создание и инициализация результатов производительности
  std::shared_ptr<ppc::core::PerfResults> perfResults = std::make_shared<ppc::core::PerfResults>();

  // Создание анализатора производительности
  std::shared_ptr<ppc::core::Perf> perfAnalyzer = std::make_shared<ppc::core::Perf>(sobel);

  // Профилирование полного пайплайна выполнения задачи
  perfAnalyzer->pipeline_run(perfAttr, perfResults);

  // Вывод статистики производительности
  ppc::core::Perf::print_perf_statistic(perfResults);
}

TEST(sobol_l_sobel_seq, test_task_run) {
  int width = 3000, height = 3000;

  // Создание тестовых данных
  std::vector<sobol::RGB> input = sobol::getRandomRGBPicture(width, height);
  std::vector<sobol::RGB> output(width * height);

  // Инициализация данных задачи
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  taskData->inputs_count.emplace_back(static_cast<uint32_t>(input.size() * sizeof(sobol::RGB)));
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  taskData->outputs_count.emplace_back(static_cast<uint32_t>(output.size() * sizeof(sobol::RGB)));

  // Создание задачи
  std::shared_ptr<sobol::Sobel_seq> sobel = std::make_shared<sobol::Sobel_seq>(taskData, width, height);

  // Создание атрибутов производительности
  std::shared_ptr<ppc::core::PerfAttr> perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  perfAttr->current_timer = [&] {
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  };

  // Создание и инициализация результатов производительности
  std::shared_ptr<ppc::core::PerfResults> perfResults = std::make_shared<ppc::core::PerfResults>();

  // Создание анализатора производительности
  std::shared_ptr<ppc::core::Perf> perfAnalyzer = std::make_shared<ppc::core::Perf>(sobel);

  // Измерение производительности выполнения задачи
  perfAnalyzer->task_run(perfAttr, perfResults);

  // Вывод статистики производительности
  ppc::core::Perf::print_perf_statistic(perfResults);
}
