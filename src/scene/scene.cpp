#include "scene.h"

#include <src/scene_items/itemsfactory.h>

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QIODevice>

#include "qmimedata.h"
#include "src/dragitem/dragitem.h"

Scene::Scene(QObject *parent) : QGraphicsScene{parent} {
  setSceneRect(QRectF(0, 0, 400, 500));
}

const QList<SceneItem *> &Scene::GetWalls() const { return walls_; }
const QList<SceneItem *> &Scene::GetCoins() const { return coins_; }
SceneItem *Scene::GetPlayer() const { return player_; }

bool Scene::DeleteCoin(SceneItem *coin) {
  if (!coins_.contains(coin)) {
    return false;
  }
  removeItem(coin);
  coins_.removeOne(coin);
  delete coin;
  return true;
}
void Scene::SetFiguresInteraction(bool enable) {
  for (auto &item : items()) {
    item->setFlag(QGraphicsItem::ItemIsMovable, enable);
    item->setFlag(QGraphicsItem::ItemIsSelectable, enable);
    item->setFlag(QGraphicsItem::ItemIsFocusable, enable);
  }
}

void Scene::RotateSelectedItems() {
  int angle = 10;

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
    if (item == player_) {
      player_ = nullptr;
    }
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

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    event->accept();
  }
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    QByteArray itemData = event->mimeData()->data(DragItem::kMimeFormat);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString itemType;
    dataStream >> itemType;
    qDebug() << itemType;
    SceneItem *item = ItemsFactory::Create(itemType);
    item->setPos(event->scenePos());

    if (itemType == "Игрок") {
      if (player_ == nullptr) {
        player_ = item;
        qDebug() << player_;
      } else {
        item = nullptr;
      }
    }

    if (itemType == "Монетка") {
      coins_.append(item);
    }

    if (itemType == "Стенка") {
      walls_.append(item);
    }
    addItem(item);
    event->acceptProposedAction();
  } else {
    event->ignore();
  }
}

void Scene::keyPressEvent(QKeyEvent *pressedKey) {
  if (player_ != nullptr) {
    switch (pressedKey->key()) {
      case Qt::Key_Left:
        qDebug() << "Влево";
        player_->SetDirection(-1, 0);
        break;
      case Qt::Key_Right:
        player_->SetDirection(1, 0);
        qDebug() << "Вправо";
        break;
      case Qt::Key_Up:
        player_->SetDirection(0, -1);
        qDebug() << "Вверх";
        break;
      case Qt::Key_Down:
        player_->SetDirection(0, 1);
        qDebug() << "Вниз";
        break;
    }
  }
}
