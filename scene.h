#ifndef SCENE_H
#define SCENE_H

#include <QKeyEvent>
#include <QGraphicsScene>
#include "pacmanitem.h"
#include "itemsfactory.h"

class Scene : public QGraphicsScene {
  // Q_OBJECT
 public:
  explicit Scene(QObject *parent = nullptr);
  const QList<PacmanItem *> &getWalls() const;
  const QList<PacmanItem *> &getCoins() const;
  PacmanItem *getPlayer() const;
  void createEvent(ItemsFactory::ItemsType itemType);
  void setFiguresDragAndDropOption(bool enable);
  bool deleteCoin(PacmanItem *coin);

 public slots:
  void RotateSelectedItems();
  void ScaleSelectedItems(qreal scaleFactor);
  void RemoveSelectedItems();
  void ResetSelectedItemsScale();


 protected:
  void keyPressEvent(QKeyEvent *event);
  void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
  void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

 private:
  QList<PacmanItem *> walls_;
  QList<PacmanItem *> coins_;
  PacmanItem *player_;
  // QTimer* pacmanMoveTimer_;
};

#endif // SCENE_H
