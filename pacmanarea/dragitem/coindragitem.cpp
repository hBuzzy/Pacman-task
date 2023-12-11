#include "coindragitem.h"

CoinDragItem::CoinDragItem(qreal x, qreal y, qreal boxSize) :
    AbstractDragItem::AbstractDragItem(x, y, boxSize, DragItemFabric::getNameByType(DragItemFabric::COIN), Qt::yellow)
{
    setRect(x, y, boxSize * WIDTH_, boxSize * HEIGHT_);
    AbstractDragItem::WIDTH_ = WIDTH_;
    AbstractDragItem::HEIGHT_ = HEIGHT_;
}

void CoinDragItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::yellow);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rect = boundingRect();
    painter->drawEllipse(rect);
}

void CoinDragItem::touch(){
    delete this;
};
