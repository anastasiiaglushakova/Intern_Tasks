#include "mainwindow.hpp"

#include <QDebug>
#include <QHeaderView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Ресурсоемкие задачи - прогресс");
  resize(400, 300);

  table = new QTableWidget(this);  // Создаём таблицу
  setCentralWidget(table);  // Делаем её центральным виджетом

  const int numTasks = 10;  // Кол-во параллельных задач
  setupTable(numTasks);     // Настройка таблицы
  startWorkers(numTasks);   // Запуск потоков
}

MainWindow::~MainWindow() {
  // Завершение всех потоков при выходе
  for (int i = 0; i < threads.size(); ++i) {
    threads[i]->quit();  // Просим поток завершиться
    threads[i]->wait();  // Ждём завершения
    delete workers[i];   // Освобождаем память
    delete threads[i];
  }
}

void MainWindow::setupTable(int numTasks) {
  table->setRowCount(numTasks);
  table->setColumnCount(2);
  table->setHorizontalHeaderLabels(QStringList() << "Задача" << "Прогресс");

  progressBars.resize(numTasks);  // Выделение места под прогресс-бары

  for (int i = 0; i < numTasks; ++i) {
    // Название задачи
    table->setItem(i, 0, new QTableWidgetItem(QString("Задача %1").arg(i + 1)));

    // Прогресс-бар в ячейке
    QProgressBar *bar = new QProgressBar(this);
    bar->setRange(0, 100);
    bar->setValue(0);
    table->setCellWidget(i, 1, bar);
    progressBars[i] = bar;
  }

  // Автоматическое изменение размера колонок
  table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  table->horizontalHeader()->setSectionResizeMode(
      1, QHeaderView::ResizeToContents);
  table->verticalHeader()->setVisible(false);
}

void MainWindow::startWorkers(int numTasks) {
  workers.resize(numTasks);
  threads.resize(numTasks);

  for (int i = 0; i < numTasks; ++i) {
    threads[i] = new QThread(this);  // Создаём поток
    workers[i] = new Worker(i, nullptr);  // Создаём воркера с уникальным ID

    workers[i]->moveToThread(threads[i]);  // Перемещаем воркера в поток

    // Подключение сигналов и слотов
    connect(threads[i], &QThread::started, workers[i], &Worker::startWork);
    connect(workers[i], &Worker::progressUpdated, this,
            &MainWindow::handleProgress);
    connect(workers[i], &Worker::workFinished, this,
            &MainWindow::handleFinished);
    connect(workers[i], &Worker::workFinished, threads[i], &QThread::quit);

    threads[i]->start();  // Запуск потока
  }
}

void MainWindow::handleProgress(int id, int progress) {
  // Обновление значения прогресс-бара
  if (id >= 0 && id < progressBars.size()) {
    progressBars[id]->setValue(progress);
  }
}

void MainWindow::handleFinished(int id) {
  // Установка текста "Завершено"
  if (id >= 0 && id < progressBars.size()) {
    progressBars[id]->setFormat("Завершено");
    progressBars[id]->setValue(100);
  }
}
