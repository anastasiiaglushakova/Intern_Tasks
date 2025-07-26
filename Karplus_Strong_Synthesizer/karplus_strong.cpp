#include "karplus_strong.hpp"
#include <fstream>
#include <cmath>
#include <iostream>
#include <deque>

// Запись WAV файла с 16-битным моно сигналом
void write_wav(const char* filename, const std::vector<int16_t>& data, uint32_t sample_rate) {
    std::ofstream f(filename, std::ios::binary);
    if (!f) {
        std::cerr << "Ошибка при открытии файла для записи\n";
        return;
    }

    // Подсчёт размеров и параметров WAV заголовка
    uint32_t data_chunk_size = data.size() * sizeof(int16_t); // Размер данных
    uint32_t fmt_chunk_size = 16;                             // Размер fmt блока
    uint32_t riff_chunk_size = 4 + (8 + fmt_chunk_size) + (8 + data_chunk_size); // Общий размер RIFF блока
    uint16_t audio_format = 1; // PCM формат
    uint16_t num_channels = 1; // Моно
    uint16_t bits_per_sample = 16;
    uint32_t byte_rate = sample_rate * num_channels * bits_per_sample / 8; // Скорость передачи данных
    uint16_t block_align = num_channels * bits_per_sample / 8;            // Размер блока

    // Записываем заголовок RIFF
    f.write("RIFF", 4);
    f.write(reinterpret_cast<const char*>(&riff_chunk_size), 4);
    f.write("WAVE", 4);

    // Записываем fmt блок с параметрами аудио
    f.write("fmt ", 4);
    f.write(reinterpret_cast<const char*>(&fmt_chunk_size), 4);
    f.write(reinterpret_cast<const char*>(&audio_format), 2);
    f.write(reinterpret_cast<const char*>(&num_channels), 2);
    f.write(reinterpret_cast<const char*>(&sample_rate), 4);
    f.write(reinterpret_cast<const char*>(&byte_rate), 4);
    f.write(reinterpret_cast<const char*>(&block_align), 2);
    f.write(reinterpret_cast<const char*>(&bits_per_sample), 2);

    // Записываем data блок с аудиоданными
    f.write("data", 4);
    f.write(reinterpret_cast<const char*>(&data_chunk_size), 4);
    f.write(reinterpret_cast<const char*>(data.data()), data_chunk_size);

    f.close();
}

// Основная функция синтеза по алгоритму Карплуса-Стронга
std::vector<int16_t> karplus_strong_synthesis(const std::vector<double>& frequencies,
                                              double sample_rate,
                                              double duration_sec,
                                              double decay) {
    std::vector<int16_t> output; // Итоговый аудиобуфер

    for (double freq : frequencies) {
        if (freq <= 0) continue; // Пропускаем невалидные частоты

        // Размер буфера для моделирования струнной вибрации
        size_t N = static_cast<size_t>(sample_rate / freq);
        if (N == 0) N = 1; // Минимум 1 сэмпл

        std::deque<double> buffer;
        // Инициализация начального буфера случайным шумом +1/-1
        for (size_t i = 0; i < N; ++i) {
            buffer.push_back((rand() % 2 == 0) ? 1.0 : -1.0);
        }

        size_t total_samples = static_cast<size_t>(sample_rate * duration_sec);

        // Генерация сэмплов звука
        for (size_t i = 0; i < total_samples; ++i) {
            double first = buffer.front();
            buffer.pop_front();
            double second = buffer.front();
            // Формула Карплуса-Стронга: среднее соседних значений с затуханием
            double new_sample = decay * 0.5 * (first + second);
            buffer.push_back(new_sample);

            // Ограничиваем амплитуду [-1;1] и конвертируем в int16_t
            int16_t out_sample = static_cast<int16_t>(std::max(-1.0, std::min(1.0, new_sample)) * 32767);
            output.push_back(out_sample);
        }
    }

    return output;
}
