#ifndef GAME_H
#define GAME_H

#include <src/dragitem/dragitem.h>
#include <src/mainwindow/mainwindow.h>
#include <src/scene/scene.h>
#include <src/scene_items/itemsfactory.h>

#include <QKeyEvent>
#include <QMessageBox>
#include <QTimer>
#include <QWidget>

class ItemsFactory;
class DragItem;
class Game : public QWidget {
  Q_OBJECT
 public:
  Game(Scene& scene, QWidget* parent = nullptr);

 public slots:
  void StartGame();
  void StopGame();
  void ShowAbout();

 signals:
  void ScoreChanged(int score);

 protected:
  void timerEvent(QTimerEvent* event);

 private:
  void WonGame();
  void UpdateScore(int addScore);
  void DeleteCoin(SceneItem* coin);
  bool HasCollision();
  bool IsOutOfBounds(int newX, int newY);

 private:
  uint intervalTime_ = 200;
  uint score_ = 0;

  Scene& scene_;
  SceneItem* player_;

  QColor collisionColor_;
  QBrush originColor_;

  QBasicTimer gameTimer_;
};

#endif  // GAME_H
