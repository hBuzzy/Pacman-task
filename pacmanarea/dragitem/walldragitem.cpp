#include "walldragitem.h"

WallDragItem::WallDragItem(qreal x, qreal y, qreal boxSize, kWallType type) :
    AbstractDragItem::AbstractDragItem(
        x,
        y,
        boxSize,
        (type == kWallType::VERTICAL ?  DragItemFabric::getNameByType(DragItemFabric::VWALL) : DragItemFabric::getNameByType(DragItemFabric::HWALL)),
        Qt::blue
    ){
    if (type == kWallType::HORISONTAL){
        qreal temporaryVariable = width_;
        WIDTH_ = height_;
        HEIGHT_ = temporaryVariable;
    }
    setRect(x, y, boxSize * width_, boxSize * height_);
    AbstractDragItem::WIDTH_ = width_;
    AbstractDragItem::HEIGHT_ = height_;
}

void WallDragItem::touch(){};
