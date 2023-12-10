#ifndef SCENE_H
#define SCENE_H

#include <src/scene_items/itemsfactory.h>

#include <QGraphicsScene>
#include <QKeyEvent>

class Scene : public QGraphicsScene {
  // Q_OBJECT
 public:
  explicit Scene(QObject *parent = nullptr);
  const QList<SceneItem *> &GetWalls() const;
  const QList<SceneItem *> &GetCoins() const;
  const QList<SceneItem *> &SetCoins(QList<SceneItem *>) const;
  SceneItem *GetPlayer() const;
  bool DeleteCoin(SceneItem *coin);
  void SetFiguresInteraction(bool enable);

 public slots:
  void RotateSelectedItems();
  void ScaleSelectedItems(qreal scaleFactor);
  void RemoveSelectedItems();
  void ResetSelectedItemsScale();

 protected:
  void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
  void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
  void dropEvent(QGraphicsSceneDragDropEvent *event);
  void keyPressEvent(QKeyEvent *event);

 private:
  QList<SceneItem *> walls_;
  QList<SceneItem *> coins_;
  SceneItem *player_;
};

#endif  // SCENE_H
