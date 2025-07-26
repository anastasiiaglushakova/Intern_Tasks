#include <cstdlib>
#include <iostream>
#include <vector>

#include "karplus_strong.hpp"

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cout
        << "Использование:\n"
        << argv[0]
        << " output.wav sample_rate duration_sec freq1 [freq2 freq3 ...]\n"
        << "Пример:\n"
        << argv[0] << " output.wav 44100 2 440 660 880\n";
    return 1;
  }

  const char* filename = argv[1];           // Имя файла вывода
  double sample_rate = std::atof(argv[2]);  // Частота дискретизации
  double duration = std::atof(argv[3]);  // Длительность звучания

  if (sample_rate <= 0 || duration <= 0) {
    std::cerr << "Ошибка: частота дискретизации и длительность должны быть "
                 "положительными\n";
    return 1;
  }

  // Чтение массива частот из аргументов
  std::vector<double> frequencies;
  for (int i = 4; i < argc; ++i) {
    double f = std::atof(argv[i]);
    if (f > 0) frequencies.push_back(f);
  }

  if (frequencies.empty()) {
    std::cerr << "Ошибка: нужно хотя бы одно положительное значение частоты\n";
    return 1;
  }

  // Генерация аудиосигнала
  auto audio = karplus_strong_synthesis(frequencies, sample_rate, duration);

  // Запись в WAV файл
  write_wav(filename, audio, static_cast<uint32_t>(sample_rate));

  std::cout << "Записано в " << filename << "\n";
  return 0;
}
