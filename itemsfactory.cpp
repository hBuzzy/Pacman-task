#include "itemsfactory.h"

ItemsFactory::ItemsFactory() {}

SceneItem *ItemsFactory::Create(ItemsType type) {
  QPainterPath path;

  QPen pen(Qt::black, 1);
  QBrush brush(GetColorByItemType(type), Qt::SolidPattern);

  if (type == ItemsType::kWall) {
    path.addRect(0, 0, 100, 20);
  } else if (type == ItemsType::kPlayer) {
    path.addEllipse(0, 0, 40, 40);
    path.addEllipse(10, 10, 5, 5);
    path.addEllipse(25, 10, 5, 5);
  } else if (type == ItemsType::kCoin) {
    path.moveTo(20, 5);
    path.lineTo(23, 15);
    path.lineTo(33, 15);
    path.lineTo(25, 20);
    path.lineTo(29, 30);
    path.lineTo(20, 24);
    path.lineTo(11, 30);
    path.lineTo(15, 20);
    path.lineTo(7, 15);
    path.lineTo(17, 15);
    path.lineTo(20, 5);
  } else {
    path.addRect(0, 0, 20, 20);
  }

  return new SceneItem(path, pen, brush);
}

QColor ItemsFactory::GetColorByItemType(ItemsType type) {
  switch (type) {
    case ItemsType::kWall:
      return Qt::cyan;
    case ItemsType::kPlayer:
      return Qt::gray;
    case ItemsType::kCoin:
      return Qt::yellow;
    default:
      return Qt::black;
  }
}
