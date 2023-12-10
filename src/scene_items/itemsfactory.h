#ifndef ITEMSFACTORY_H
#define ITEMSFACTORY_H

#include <QObject>
#include <QRectF>

#include "src/scene_items/sceneitem.h"

class ItemsFactory : public QObject {
 private:
  ItemsFactory();

 public:
  static SceneItem* Create(const QString& type);
  static QColor GetColorByItemTypeconst(const QString& type);
  static inline const QString kBorder = "Стенка";
  static inline const QString kPlayer = "Игрок";
  static inline const QString kCoin = "Монетка";
};

#endif  // ITEMSFACTORY_H
