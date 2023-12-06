#ifndef PACMANGAME_H
#define PACMANGAME_H

#include <QKeyEvent>
#include <QTimer>
#include <QWidget>

#include "itemsfactory.h"
#include "scene.h"

class PacmanGame : public QWidget {
 Q_OBJECT

 public:
   PacmanGame(Scene& scene, QWidget* parent = nullptr);

 public slots:
   void startNewGame();
   void stopGame();

 signals:
   void initScoreChanged(uint score);

 protected:
   void timerEvent(QTimerEvent* event);

 private:
   void initGameEnd();
   void updateScore(int addScore);
   void deleteCoin(PacmanItem* coin);
   bool hasCollision();
   bool isOutOfBounds(int newX, int newY);


 private:
   uint score_;

   Scene& scene_;
   PacmanItem* player_;

   QColor collisionColor_;
   QColor originColor_;
   const uint intervalTime_ = 10;
   QBasicTimer gameTimer_;
};

#endif // PACMANGAME_H
