#include "game.h"

#include <QDebug>
#include <QEvent>
#include <QString>

Game::Game(Scene &scene, QWidget *parent) : QWidget(parent), scene_(scene) {
  collisionColor_ = Qt::red;
  originColor_ = Qt::yellow;
}

void Game::StartGame() {
  player_ = scene_.GetPlayer();
  if (player_ == nullptr) {
    return;
  } else {
    MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());

    if (mainWindow) {
      mainWindow->SetListWidgetEnabled(false);
    }

    gameTimer_.start(intervalTime_, this);
    qDebug() << player_;
  }
}

void Game::StopGame() {
  if (!gameTimer_.isActive()) {
    return;
  }
  MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());
  qDebug() << parent();
  if (mainWindow) {
    mainWindow->SetListWidgetEnabled(true);
  }
  gameTimer_.stop();
}

void Game::ShowAbout() {
  QMessageBox::information(
      this, "Справка",
      "Нажимаем кнопочку начать игру и тыкаем влево/вправо/вверх/вниз в "
      "зависимости от того,куда хотите его отправить");
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

void Game::WonGame() {
  scene_.SetFiguresInteraction(true);
  QMessageBox::information(
      this, "Так держать!!!",
      "Вас конечно зовут не Виктор, но здесь вы победили)");
  MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());
  qDebug() << parent();
  if (mainWindow) {
    mainWindow->SetListWidgetEnabled(true);
  }
  score_ = 0;
  UpdateScore(score_);
}
void Game::UpdateScore(int score) {
  score_ += score;
  qDebug() << score_;
  emit ScoreChanged(score_);
}

void Game::DeleteCoin(SceneItem *coin) {
  if (scene_.DeleteCoin(coin)) {
    int score = 1;
    UpdateScore(score);
    score = score + 1;
    if (scene_.GetCoins().isEmpty()) {
      WonGame();
      StopGame();
    }
  }
}

bool Game::HasCollision() {
  const QList<SceneItem *> &walls = scene_.GetWalls();
  const QList<SceneItem *> &coins = scene_.GetCoins();

  for (SceneItem *wall : walls) {
    if (player_->collidesWithItem(wall)) {
      return true;
    }
  }

  for (SceneItem *coin : coins) {
    if (player_->collidesWithItem(coin)) {
      DeleteCoin(coin);
    }
  }

  player_->ChangeBrush(originColor_);
  return false;
}

bool Game::IsOutOfBounds(int newX, int newY) {
  qDebug() << newX << newY;

  const QRectF sceneRect = scene_.sceneRect();
  qDebug() << sceneRect;
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
