#ifndef ITEMSFACTORY_H
#define ITEMSFACTORY_H

#include <QObject>
#include <QRectF>
#include <QPainter>

#include "pacmanitem.h"

class ItemsFactory : public QObject {
 public:
  enum ItemsType { kWall = 0, kPlayer = 1, kCoin = 2 };
  Q_ENUM(ItemsType)

 private:
  ItemsFactory();

 public:
  static PacmanItem* Create(ItemsType type);
  static QColor GetColorByItemType(ItemsType type);
};


#endif // ITEMSFACTORY_H
