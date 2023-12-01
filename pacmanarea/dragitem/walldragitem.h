#ifndef WALLDRAGITEM_H
#define WALLDRAGITEM_H

#include "abstractdragitem.h"
#include "dragitemfabric.h"

class WallDragItem : public AbstractDragItem {
public:
    enum WallType { VERTICAL, HORISONTAL };
public:
    WallDragItem(qreal x, qreal y, qreal boxSize, WallType type);

public slots:
    void touch();

private:
    int WIDTH_ = 1;
    int HEIGHT_ = 3;
};

#endif // WALLDRAGITEM_H
