#include "pacmangame.h"

#include <mainwindow.h>
#include <qmath.h>

#include <QDebug>
#include <QMessageBox>

PacmanGame::PacmanGame(Scene &scene, QWidget *parent) : QWidget(parent), scene_(scene) {
  collisionColor_ = Qt::red;
  originColor_ =
      ItemsFactory::GetColorByItemType(ItemsFactory::ItemsType::kPlayer);
}

void PacmanGame::startNewGame() {
  player_ = scene_.getPlayer();

  if (!player_) {
    QMessageBox::critical(this, "Error", "Поместите Пакмана на игровое поле!");
    return;
  }

  MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());

  const QList<PacmanItem *> &coins = scene_.getCoins();

  if (coins.isEmpty()) {
    if (mainWindow) {
      QMessageBox::critical(this, "Error", "Необходимо разместить монеты на игровом поле!");
      //mainWindow->SetWidgetMoveEnabled(true);
    }
    return;
  }

  if (gameTimer_.isActive()) {
    QMessageBox::critical(this, "Error", "Игра стартовала!");
    return;
  }

  score_ = 0;
  updateScore(score_);
  gameTimer_.start(intervalTime_, this);
  player_ = scene_.getPlayer();
}

void PacmanGame::stopGame() {
  if (!gameTimer_.isActive()) {
    QMessageBox::critical(this, "Error", "Игра стартовала!");
    return;
  }
  gameTimer_.stop();
}

void PacmanGame::updateScore(int score) {
  score_ += score;
  emit initScoreChanged(score_);
}

void PacmanGame::initGameEnd() {
  scene_.setFiguresDragAndDropOption(true);

  MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());
  if (mainWindow) {
    mainWindow->SetWidgetMoveEnabled(true);
  }

  score_ = 0;

  QMessageBox::information(this, "Победа!", "Победа!");
}

void PacmanGame::timerEvent(QTimerEvent *event) {
  if (event->timerId() == gameTimer_.timerId() && player_) {
    int newX = player_->pos().x() + player_->getCoordinateX() * player_->getStep();
    int newY = player_->pos().y() + player_->getCoordinateY() * player_->getStep();

    if (isOutOfBounds(newX, newY) || hasCollision()) {
      player_->invertCoordinates();
    }
    player_->movePlayer();
  } else {
    QWidget::timerEvent(event);
  }
}

bool PacmanGame::isOutOfBounds(int newX, int newY) {
  const QRectF sceneRect = scene_.sceneRect();
  QRectF playerRect(newX, newY, player_->sceneBoundingRect().width(), player_->sceneBoundingRect().height());

  if (newX < sceneRect.left() ||
      newX + playerRect.width() > sceneRect.right() || newY < sceneRect.top() ||
      newY + playerRect.height() > sceneRect.bottom()) {
    return true;
  }

  return false;
}

bool PacmanGame::hasCollision() {
  const QList<PacmanItem *> &walls = scene_.getWalls();
  const QList<PacmanItem *> &coins = scene_.getCoins();

  for (PacmanItem *wall : walls) {
    if (player_->collidesWithItem(wall)) {
      return true;
    }
  }

  for (PacmanItem *coin : coins) {
    if (player_->collidesWithItem(coin)) {
      deleteCoin(coin);
    }
  }
  return false;
}

void PacmanGame::deleteCoin(PacmanItem *coin) {
  if (scene_.deleteCoin(coin)) {
    int score = 1;
    updateScore(score);

    if (scene_.getCoins().isEmpty()) {
      initGameEnd();
      stopGame();
    }
  }
}
