#include "itemsfactory.h"

ItemsFactory::ItemsFactory() {}

SceneItem *ItemsFactory::Create(const QString &type) {
  if (type == kBorder) {
    return new SceneItem(QRectF(0, 0, 70, 40), QPen(Qt::black, 1),
                         QBrush(Qt::green, Qt::SolidPattern), type);
  } else if (type == kCoin) {
    return new SceneItem(QRectF(0, 0, 40, 40), QPen(Qt::black),
                         QBrush(QColor(255, 215, 0), Qt::SolidPattern), type);
  } else {
    return new SceneItem((QRectF(0, 0, 60, 60)), QPen(),
                         QBrush(Qt::yellow, Qt::SolidPattern), type);
  }
}

QColor ItemsFactory::GetColorByItemTypeconst(const QString &type) {
  if (type == kBorder) {
    return Qt::gray;
  } else if (type == kCoin) {
    return Qt::cyan;
  } else {
    return Qt::yellow;
  }
}
