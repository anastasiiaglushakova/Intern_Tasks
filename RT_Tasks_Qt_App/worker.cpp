#include "worker.hpp"
#include <QThread>

Worker::Worker(int id, QObject *parent)
    : QObject(parent), m_id(id)
{
    Q_UNUSED(parent) // Явно указываем, что parent не используется
}
// Запускает doWork() асинхронно в правильном потоке через очередь событий Qt
void Worker::startWork()
{
    QMetaObject::invokeMethod(this, "doWork", Qt::QueuedConnection);
}

void Worker::doWork()
{
    const int totalSteps = 100000000;              // Общее количество шагов работы (объём задачи)
    const int updateInterval = totalSteps / 100;   // Интервал обновления прогресса (каждый 1%)

    // Чем больше идентификатор задачи, тем дольше пауза — имитация разной нагрузки
    int sleepMs = 5 + m_id * 5;

    // Основной цикл вычислений
    for (int i = 0; i <= totalSteps; ++i) {
        if (i % updateInterval == 0) {             // Каждое обновление прогресса
            int progress = (i / updateInterval);  // Рассчитываем текущий прогресс в процентах
            emit progressUpdated(m_id, progress); // Посылаем сигнал с обновлённым прогрессом
            QThread::msleep(sleepMs);              // Задержка — имитация затрат времени на работу
        }
    }

    emit progressUpdated(m_id, 100);               // Посылаем сигнал о 100% завершении
    emit workFinished(m_id);                        // Посылаем сигнал о завершении работы задачи
}