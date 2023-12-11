#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <scene.h>
#include <itemsfactory.h>

#include <QMainWindow>
#include "pacmangame.h"

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
  void SetWidgetMoveEnabled(bool enabled);
  void addItemToList(ItemsFactory::ItemsType itemType);

 private slots:
  void UpdateScoreLSD(uint newScore);
  void AddNewItem();
  void AddNewPlayerItem();
  void AddNewCoinItem();
  void AddNewWallItem();

 protected:
  void keyPressEvent(QKeyEvent *event);

 private:
  void SetConnections();
  void SetUi();

 private:
  Ui::MainWindow *ui_;
  Scene *scene_;
  PacmanGame *game_;
  int rotationAngle_ = 90;
  float itemScaleFactor_ = 0.1;
};

#endif // MAINWINDOW_H
