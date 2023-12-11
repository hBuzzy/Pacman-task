#ifndef DRAGITEMFABRIC_H
#define DRAGITEMFABRIC_H

#include "abstractdragitem.h"
#include "coindragitem.h"
#include "walldragitem.h"

class DragItemFabric {
public:
    enum DragItemType { VWALL, HWALL, COIN };
public:
    static QString getNameByType(DragItemType type);
    static DragItemType getTypeByName(QString name);
    static int getNumByType(DragItemType type);
    static DragItemType getTypeByNum(int name);
    static AbstractDragItem* getVerticalWall(qreal x, qreal y, qreal boxSize);
    static AbstractDragItem* getCoin(qreal x, qreal y, qreal boxSize);
    static AbstractDragItem* getHorisontalWall(qreal x, qreal y, qreal boxSize);
    static AbstractDragItem* getItemByType(DragItemType type, qreal x, qreal y, qreal boxSize);
};

#endif // DRAGITEMFABRIC_H
