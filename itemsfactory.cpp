#include "itemsfactory.h"

ItemsFactory::ItemsFactory() {}

// Крайне примитивная реализация фабричного метода для выделения одного места,
// ответственного за создание фигур
PacmanItem *ItemsFactory::Create(ItemsType type) {
  QPainterPath figurePath;

  QPen pen(Qt::black, 1);
  QBrush brush(GetColorByItemType(type), Qt::SolidPattern);


  switch (type) {
    case ItemsType::kWall:
      figurePath.addRect(0, 0, 70, 20);
      break;

    case ItemsType::kCoin:
      figurePath.addEllipse(0, 0, 20, 20);
      break;

    case ItemsType::kPlayer:
      figurePath.addEllipse(0, 0, 40, 40);
      break;

    default:
      figurePath.addRect(0, 0, 70, 20);
  }

  return new PacmanItem(figurePath, pen, brush);
}

QColor ItemsFactory::GetColorByItemType(ItemsType type) {
  switch (type) {
    case ItemsType::kWall:
      return Qt::gray;

    case ItemsType::kPlayer:
      return Qt::red;

    case ItemsType::kCoin:
      return Qt::yellow;

    default:
      return Qt::black;
  }
}
