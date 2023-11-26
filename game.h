#ifndef GAME_H
#define GAME_H

#include <QKeyEvent>
#include <QTimer>
#include <QWidget>

#include "itemsfactory.h"
#include "scene.h"

class ItemsFactory;

class Game : public QWidget {
  Q_OBJECT

 public:
  Game(Scene& scene, QWidget* parent = nullptr);

 public slots:
  void StartNewGame();
  void StopGame();

 signals:
  void ScoreChanged(int score);

 protected:
  void timerEvent(QTimerEvent* event);

 private:
  void GameWon();
  void UpdateScore(int addScore);
  void DeleteCoin(SceneItem* coin, QList<SceneItem*>& coins);
  bool HasCollision();
  bool IsOutOfBounds(int newX, int newY);

 private:
  uint intervalTime_ = 15;
  uint score_;

  Scene& scene_;
  SceneItem* player_;

  QColor collisionColor_;
  QColor originColor_;

  QBasicTimer gameTimer_;
};

#endif  // GAME_H
