#ifndef ITEMSFACTORY_H
#define ITEMSFACTORY_H

#include <QObject>
#include <QRectF>

#include "sceneitem.h"

class SceneItem;

class ItemsFactory : public QObject {
  Q_OBJECT
 public:
  enum ItemsType { kWall = 0, kPlayer = 1, kCoin = 2 };
  Q_ENUM(ItemsType)

 private:
  ItemsFactory();

 public:
  static SceneItem* Create(ItemsType type);
  static QColor GetColorByItemType(ItemsType type);
};

#endif  // ITEMSFACTORY_H
