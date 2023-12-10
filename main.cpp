#include <src/mainwindow/mainwindow.h>

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowTitle("Да начнутся голодные игры");
  w.show();
  return a.exec();
}
