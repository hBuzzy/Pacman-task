// customgraphicsview.cpp
#include "customgraphicsview.h"

#include <QMouseEvent>
#include <QDebug>

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, QWidget *parent, int gridSize)
    : QGraphicsView(scene, parent), gridSize(gridSize)  // Добавьте инициализацию gridSize
{
    setMouseTracking(true);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (isdrawing) {
        QPoint localMousePos = event->pos();
        center = this->mapToScene(localMousePos);
        center.setX(this->center.x() - gridSize / 2);
        center.setY(this->center.y() - gridSize / 2);
        update();
        qDebug() << "cursor Coordinates: (" << center.x() << ", " << center.y() << ")";
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isdrawing) {
        isdrawing = false;
        setCursorStyle();
        emit customMouseRelease();
    }
}

void CustomGraphicsView::setCursorStyle() {
    if (isdrawing) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}
