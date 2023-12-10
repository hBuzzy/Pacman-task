#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <src/dragitem/dragitem.h>
#include <src/game.h>
#include <src/scene/scene.h>
#include <src/scene_items/itemsfactory.h>

#include <QMainWindow>
class Game;
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
  void AddItemToList(const QString &type);

 private:
  Ui::MainWindow *ui_;
  Scene *scene_;
  int rotationAngle_ = 10;
  qreal itemScaleFactor_ = 0.1;
  Game *game_;
  SceneItem *player_;
};
#endif  // MAINWINDOW_H
