#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTableWidget>
#include <QProgressBar>
#include <QVector>
#include <QThread>
#include "worker.hpp"

// Основной графический интерфейс
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr); // Конструктор
    ~MainWindow();                                  // Деструктор — завершает потоки

private slots:
    void handleProgress(int id, int progress); // Обработка сигнала обновления прогресса
    void handleFinished(int id);               // Обработка сигнала завершения задачи

private:
    QTableWidget *table;                 // Таблица для отображения задач
    QVector<QProgressBar*> progressBars; // Индикаторы прогресса по строкам
    QVector<QThread*> threads;           // Потоки для каждой задачи
    QVector<Worker*> workers;            // Объекты-исполнители

    void setupTable(int numTasks);       // Инициализация таблицы
    void startWorkers(int numTasks);     // Запуск задач
};

#endif // MAINWINDOW_HPP
