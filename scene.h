#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

#include "sceneitem.h"

class Scene : public QGraphicsScene {
 public:
  explicit Scene(QObject *parent = nullptr);

  QList<SceneItem *> &GetWalls();
  QList<SceneItem *> &GetCoins();
  SceneItem *GetPlayer() const;

  void SetFiguresInteraction(bool enable);

 public slots:
  void RotateSelectedItems();
  void RemoveSelectedItems();

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
