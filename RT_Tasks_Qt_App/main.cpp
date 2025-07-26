#include <QApplication>

#include "mainwindow.hpp"

// Точка входа в приложение
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);  // Инициализация Qt-приложения

  MainWindow w;  // Создание основного окна
  w.show();      // Отображение окна

  return app.exec();  // Запуск основного цикла обработки событий
}
