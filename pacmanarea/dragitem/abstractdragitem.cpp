#include "abstractdragitem.h"
#include <QDebug>

AbstractDragItem::AbstractDragItem(){};

AbstractDragItem::AbstractDragItem(qreal x, qreal y, qreal boxSize, QString name, QColor color) {
    setBrush(color);
    setFlag(QGraphicsItem::ItemIsMovable);
    this->name_ = name;
}

void AbstractDragItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QMimeData* mimeData = new QMimeData;
        mimeData->setText(name_);

        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);

        drag->exec(Qt::MoveAction);
    }

    QGraphicsRectItem::mousePressEvent(event);
}

QString AbstractDragItem::getName() {
    return name_;
};

QPair<int, int> AbstractDragItem::getSize() {
    return QPair<int, int>{ WIDTH_, HEIGHT_ };
}
