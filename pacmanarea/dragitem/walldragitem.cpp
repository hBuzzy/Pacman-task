#include "walldragitem.h"

WallDragItem::WallDragItem(qreal x, qreal y, qreal boxSize, WallType type) :
    AbstractDragItem::AbstractDragItem(
        x,
        y,
        boxSize,
        (type == WallType::VERTICAL ?  DragItemFabric::getNameByType(DragItemFabric::VWALL) : DragItemFabric::getNameByType(DragItemFabric::HWALL)),
        Qt::blue
    ){
    if(type == WallType::HORISONTAL){
        qreal temp = WIDTH_;
        WIDTH_ = HEIGHT_;
        HEIGHT_ = temp;
    }
    setRect(x, y, boxSize * WIDTH_, boxSize * HEIGHT_);
    AbstractDragItem::WIDTH_ = WIDTH_;
    AbstractDragItem::HEIGHT_ = HEIGHT_;
}

void WallDragItem::touch(){};
