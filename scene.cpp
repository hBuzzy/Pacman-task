#include "scene.h"

#include <itemsfactory.h>

#include <QByteArray>
#include <QDataStream>
#include <QGraphicsSceneDragDropEvent>
#include <QIODevice>
#include <QTimer>
#include <QMessageBox>

#include "qmimedata.h"
#include "dragitem.h"

Scene::Scene(QObject *parent) : QGraphicsScene{parent} { player_ = nullptr; }

void Scene::keyPressEvent(QKeyEvent *event) {
  int deltaTimerInterval = 50;
  int positionFirst = 2;
  int positionSecond = 0;

  if (event->key() == Qt::Key_Delete) {
    RemoveSelectedItems();

  } else if (event->key() == Qt::Key_Down) {
    player_->setCoordinates(0, 1);

  } else if (event->key() == Qt::Key_Up) {
   player_->setCoordinates(0, -1);

  } else if (event->key() == Qt::Key_Left) {
    player_->setCoordinates(-1, 0);

  } else if (event->key() == Qt::Key_Right) {
    player_->setCoordinates(1, 0);
  }
}

const QList<PacmanItem *> &Scene::getWalls() const { return walls_; }
const QList<PacmanItem *> &Scene::getCoins() const { return coins_; }
PacmanItem *Scene::getPlayer() const { return player_; }

bool Scene::deleteCoin(PacmanItem *coin) {
  if (!coins_.contains(coin)) {
    return false;
  }
  removeItem(coin);
  coins_.removeOne(coin);
  delete coin;
  return true;
}

void Scene::RotateSelectedItems() {
  int angle = 90;

  for (auto &item : selectedItems()) {
    item->setRotation(item->rotation() + angle);
  }
}

void Scene::ScaleSelectedItems(qreal scaleFactor) {
  for (auto &item : selectedItems()) {
    item->setScale(item->scale() + scaleFactor);
  }
}

void Scene::RemoveSelectedItems() {
  for (auto &item : selectedItems()) {
    removeItem(item);
  }
}

void Scene::ResetSelectedItemsScale() {
  int defaultScale = 1;

  for (auto &item : selectedItems()) {
    item->setScale(defaultScale);
  }
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    event->acceptProposedAction();
  } else {
    event->ignore();
  }
}

void Scene::setFiguresDragAndDropOption(bool enable) {
    for (auto &item : items()) {
      qDebug() << item;
      item->setFlag(QGraphicsItem::ItemIsMovable, enable);
      item->setFlag(QGraphicsItem::ItemIsSelectable, enable);
      item->setFlag(QGraphicsItem::ItemIsFocusable, enable);
    }
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    event->accept();
  }
}

void Scene::createEvent(ItemsFactory::ItemsType itemType) {
  PacmanItem *item = ItemsFactory::Create(itemType);

  switch (itemType) {
    case ItemsFactory::ItemsType::kWall:
      walls_.append(item);
      break;
    case ItemsFactory::ItemsType::kCoin:
      coins_.append(item);
      break;
    case ItemsFactory::ItemsType::kPlayer:
      if (player_ == nullptr) {
        player_ = item;
      } else {
          QMessageBox::information(nullptr, "Ошибка", "На сцене может быть только один игрок.");
          return;
        }
       break;

    default:
      break;
  }

  addItem(item);
}




