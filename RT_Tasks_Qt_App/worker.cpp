#include "worker.hpp"

#include <QThread>

Worker::Worker(int id, QObject *parent)
    : QObject(parent),
      m_id(id)  // Инициализация константы
{}

// Запускает doWork() асинхронно в правильном потоке через очередь событий Qt
void Worker::startWork() {
  if (!QMetaObject::invokeMethod(this, "doWork", Qt::QueuedConnection)) {
    qWarning("Failed to invoke doWork() for worker %d", m_id);
  }
}

void Worker::doWork() {
  const int totalSteps =
      100000000;  // Общее количество шагов работы (объём задачи)
  const int updateInterval =
      totalSteps / 100;  // Интервал обновления прогресса (каждый 1%)

  // Чем больше идентификатор задачи, тем дольше пауза — имитация разной
  // нагрузки
  const int sleepMs = 5 + m_id * 5;

  // Основной цикл вычислений

  try {
    for (int i = 0; i <= totalSteps; ++i) {
      if (i % updateInterval == 0) {  // Каждое обновление прогресса
        emit progressUpdated(
            m_id,
            i / updateInterval);  // Посылаем сигнал с обновлённым прогрессом
        QThread::msleep(
            sleepMs);  // Задержка — имитация затрат времени на работу

        // Проверка на прерывание
        if (QThread::currentThread()->isInterruptionRequested()) {
          emit progressUpdated(m_id, -1);  // Сигнал отмены
          return;
        }
      }
    }
    emit progressUpdated(m_id, 100);  // Посылаем сигнал о 100% завершении
  } catch (...) {
    emit progressUpdated(m_id, -2);  // Сигнал ошибки
  }

  emit workFinished(m_id);  // Посылаем сигнал о завершении работы задачи
}