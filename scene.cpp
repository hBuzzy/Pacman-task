#include "scene.h"

#include <itemsfactory.h>

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QIODevice>
#include <QKeyEvent>
#include <QMessageBox>

#include "dragitem.h"
#include "qmimedata.h"

Scene::Scene(QObject *parent) : QGraphicsScene{parent} { player_ = nullptr; }

QList<SceneItem *> &Scene::GetWalls() { return walls_; }

QList<SceneItem *> &Scene::GetCoins() { return coins_; }

SceneItem *Scene::GetPlayer() const { return player_; }

void Scene::RotateSelectedItems() {
  int angle = 90;

  for (auto &item : selectedItems()) {
    item->setRotation(item->rotation() + angle);
  }
}

void Scene::RemoveSelectedItems() {
  for (auto &item : selectedItems()) {
    removeItem(item);

    auto wallsIterator = std::find(walls_.begin(), walls_.end(), item);
    if (wallsIterator != walls_.end()) {
      walls_.erase(wallsIterator);
    }

    auto coinsIterator = std::find(coins_.begin(), coins_.end(), item);
    if (coinsIterator != coins_.end()) {
      coins_.erase(coinsIterator);
    }

    if (item == player_) {
      player_ = nullptr;
    }
  }
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    event->acceptProposedAction();
  } else {
    event->ignore();
  }
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    event->accept();
  }
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    QByteArray itemData = event->mimeData()->data(DragItem::kMimeFormat);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    ItemsFactory::ItemsType itemType;
    dataStream >> itemType;

    SceneItem *item = ItemsFactory::Create(itemType);

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
          QMessageBox::information(nullptr, "Ошибка",
                                   "На сцене может быть только один игрок.");
          event->ignore();
          return;
        }
        break;

      default:
        break;
    }

    item->setPos(event->scenePos());
    addItem(item);
    event->acceptProposedAction();
  } else {
    event->ignore();
  }
}

void Scene::keyPressEvent(QKeyEvent *event) {
  QList<QGraphicsItem *> selectedItemsList = selectedItems();
  if (event->key() == Qt::Key_Delete) {
    if (!selectedItemsList.isEmpty()) {
      RemoveSelectedItems();
      qDebug() << "Объекты удалены";
    }
  }

  if (event->key() == Qt::Key_R) {
    if (!selectedItemsList.isEmpty()) {
      RotateSelectedItems();
      qDebug() << "Объекты повернуты";
    }
  }

  if (event->key() == Qt::Key_Left) {
    player_->SetDirection(-1, 0);
  }

  if (event->key() == Qt::Key_Right) {
    player_->SetDirection(1, 0);
  }

  if (event->key() == Qt::Key_Down) {
    player_->SetDirection(0, 1);
  }

  if (event->key() == Qt::Key_Up) {
    player_->SetDirection(0, -1);
  }
}

void Scene::SetFiguresInteraction(bool enable) {
  for (auto &item : items()) {
    item->setFlag(QGraphicsItem::ItemIsMovable, enable);
    item->setFlag(QGraphicsItem::ItemIsSelectable, enable);
    item->setFlag(QGraphicsItem::ItemIsFocusable, enable);
  }
}
