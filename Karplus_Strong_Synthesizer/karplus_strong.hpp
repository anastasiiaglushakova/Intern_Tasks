#pragma once
#include <vector>
#include <cstdint>

// Функция записи массива сэмплов в WAV файл с заданной частотой дискретизации
void write_wav(const char* filename, const std::vector<int16_t>& data, uint32_t sample_rate);

// Функция синтеза звука алгоритмом Карплуса-Стронга
// frequencies - массив частот (Гц)
// sample_rate - частота дискретизации (Гц)
// duration_sec - длительность звучания в секундах
// decay - коэффициент затухания, по умолчанию 0.996
std::vector<int16_t> karplus_strong_synthesis(const std::vector<double>& frequencies,
                                              double sample_rate,
                                              double duration_sec,
                                              double decay = 0.996);
