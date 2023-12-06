#include "scene.h"

#include <itemsfactory.h>

#include <QByteArray>
#include <QDataStream>
#include <QGraphicsSceneDragDropEvent>
#include <QIODevice>
#include <QTimer>

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

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    event->accept();
  }
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat(DragItem::kMimeFormat)) {
    /*
     *Если формат mime нам подходит, то считываем необходимые данные (тип
     *фигуры), которые Вносили в DragItem
     */
    QByteArray itemData = event->mimeData()->data(DragItem::kMimeFormat);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    ItemsFactory::ItemsType itemType;
    // Записываем значение переданного типа из потока.
    dataStream >> itemType;

    // Создаем объект согласно типу и размещаем его относительно положения мыши
    // в момент отпускания левой кнопки мыши
    PacmanItem *item = ItemsFactory::Create(itemType);
    item->setPos(event->scenePos());

    // Добавляем объект на сцену
    addItem(item);

    event->acceptProposedAction();
  } else {
    event->ignore();
  }
}




