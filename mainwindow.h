#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <game.h>
#include <itemsfactory.h>
#include <scene.h>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void SetListWidgetEnabled(bool enabled);

 private slots:
  void UpdateScoreLabel(int newScore);

 private:
  void SetConnections();
  void SetUi();
  void AddItemToList(ItemsFactory::ItemsType itemType);
  void ShowHelp();

 private:
  Ui::MainWindow *ui_;
  Scene *scene_;
  Game *game_;
};
#endif  // MAINWINDOW_H
