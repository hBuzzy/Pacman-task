#ifndef WALLDRAGITEM_H
#define WALLDRAGITEM_H

#include "abstractdragitem.h"
#include "dragitemfabric.h"

class WallDragItem : public AbstractDragItem {
public:
    enum kWallType { VERTICAL, HORISONTAL };
public:
    WallDragItem(qreal x, qreal y, qreal boxSize, WallType type);

public slots:
    void touch();

private:
    int width_ = 1;
    int height_ = 3;
};

#endif // WALLDRAGITEM_H
