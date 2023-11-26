#include "game.h"

#include <mainwindow.h>
#include <qmath.h>

#include <QDebug>
#include <QMessageBox>

Game::Game(Scene &scene, QWidget *parent) : QWidget(parent), scene_(scene) {
  collisionColor_ = Qt::red;
  originColor_ =
      ItemsFactory::GetColorForItemType(ItemsFactory::ItemsType::kPlayer);
}

void Game::StartNewGame() {
  player_ = scene_.GetPlayer();

  if (!player_) {
    QMessageBox::critical(this, "Ошибка!",
                          "Необходимо разместить игрока на сцену");
    return;
  }

  MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());

  QList<SceneItem *> &coins = scene_.GetCoins();
  if (coins.isEmpty()) {
    if (mainWindow) {
      QMessageBox::critical(this, "Ошибка!",
                            "Необходимо разместить хотя бы 1 звездочку");
      mainWindow->SetListWidgetEnabled(true);
    }
    return;
  }

  if (gameTimer_.isActive()) {
    QMessageBox::critical(this, "Ошибка!", "Игра уже началась");
    return;
  }

  if (mainWindow) {
    mainWindow->SetListWidgetEnabled(false);
  }

  score_ = 0;
  UpdateScore(score_);
  gameTimer_.start(intervalTime_, this);
  player_ = scene_.GetPlayer();
}

void Game::StopGame() {
  if (!gameTimer_.isActive()) {
    QMessageBox::critical(this, "Ошибка!", "Игра еще не началась");
    return;
  }
  gameTimer_.stop();
}

void Game::UpdateScore(int score) {
  score_ += score;
  emit ScoreChanged(score_);
}

void Game::HandleGameWon() {
  scene_.SetFiguresInteraction(true);

  MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());
  if (mainWindow) {
    mainWindow->SetListWidgetEnabled(true);
  }
  QMessageBox::information(this, "Congratulations!",
                           "Все звездочки собраны. Так держать!");
  score_ = 0;
  UpdateScore(score_);
}

void Game::timerEvent(QTimerEvent *event) {
  if (event->timerId() == gameTimer_.timerId() && player_) {
    int newX =
        player_->pos().x() + player_->GetDirectionX() * player_->GetStep();
    int newY =
        player_->pos().y() + player_->GetDirectionY() * player_->GetStep();

    if (IsOutOfBounds(newX, newY) || HasCollision()) {
      player_->ChangeBrush(collisionColor_);
      player_->InvertDirection();
    }
    player_->Move();
  } else {
    QWidget::timerEvent(event);
  }
}

bool Game::IsOutOfBounds(int newX, int newY) {
  const QRectF sceneRect = scene_.sceneRect();
  QRectF playerRect(newX, newY, player_->sceneBoundingRect().width(),
                    player_->sceneBoundingRect().height());

  if (newX < sceneRect.left() ||
      newX + playerRect.width() > sceneRect.right() || newY < sceneRect.top() ||
      newY + playerRect.height() > sceneRect.bottom()) {
    qDebug() << "Игрок достиг границы";
    return true;
  }

  return false;
}

bool Game::HasCollision() {
  QList<SceneItem *> &walls = scene_.GetWalls();
  QList<SceneItem *> &coins = scene_.GetCoins();

  for (SceneItem *wall : walls) {
    if (player_->collidesWithItem(wall)) {
      return true;
    }
  }

  for (SceneItem *coin : coins) {
    if (player_->collidesWithItem(coin)) {
      DeleteCoin(coin, coins);
    }
  }

  player_->ChangeBrush(originColor_);
  return false;
}

void Game::DeleteCoin(SceneItem *coin, QList<SceneItem *> &coins) {
  scene_.removeItem(coin);
  coins.removeOne(coin);
  delete coin;

  int score = 1;
  UpdateScore(score);

  if (coins.isEmpty()) {
    HandleGameWon();
    StopGame();
  }
}
