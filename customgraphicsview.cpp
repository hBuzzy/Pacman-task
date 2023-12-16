#include "customgraphicsview.h"

#include <QMouseEvent>
#include <QDebug>

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, QWidget *parent, int gridSize)
    : QGraphicsView(scene, parent), cellSize_(gridSize)
{
    setMouseTracking(true);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (isDrawing_)
    {
        int halfGridSize = cellSize_ / 2;
        QPoint localMousePos = event->pos();
        center_ = this->mapToScene(localMousePos);
        center_.setX(this->center_.x() - halfGridSize);
        center_.setY(this->center_.y() - halfGridSize);
        update();
        qDebug() << "cursor Coordinates: (" << center_.x() << ", " << center_.y() << ")";
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing_)
    {
        isDrawing_ = false;
        setCursorStyle();
        emit customMouseRelease();
    }
}

void CustomGraphicsView::setCursorStyle()
{
    if (isDrawing_)
    {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

int CustomGraphicsView::getCenterX() const
{
    return center_.x();
}

int CustomGraphicsView::getCenterY() const
{
    return center_.y();
}

void CustomGraphicsView::setCenterX(int newX)
{
    center_.setX(newX);
}

void CustomGraphicsView::setCenterY(int newY)
{
    center_.setY(newY);
}

void CustomGraphicsView::setCenter(QPointF newCenter)
{
    center_ = newCenter;
}

void CustomGraphicsView::setDrawing(bool newDrawing)
{
    isDrawing_ = newDrawing;
}
