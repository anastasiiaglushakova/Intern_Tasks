#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <QThread>

// Класс "тяжёлой задачи", работающей в своём потоке
class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(int id, QObject *parent = nullptr); // Конструктор
    void startWork();                                   // Метод для отложенного запуска

signals:
    void progressUpdated(int id, int progress); // Сигнал прогресса
    void workFinished(int id);                  // Сигнал завершения

public slots:
    void doWork(); // Основной расчёт

private:
    int m_id; // Уникальный идентификатор задачи
};

#endif // WORKER_HPP
