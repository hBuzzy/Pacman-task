#ifndef DROPPABLEAREA_H
#define DROPPABLEAREA_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QMap>
#include <QtMath>
#include <QGraphicsScene>
#include <QTimer>
#include "../dragitem/dragitemfabric.h"
#include "../pacman/pacmanitem.h"
#include <QMessageBox>
#include <memory>


class DroppableArea : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    DroppableArea(int x, int y, qreal width, qreal height);
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;

public slots:
    void initGame();
    void closeGame();
    void updateGame();

signals:
    void updateScore();

private:
    QPoint getBoxCoord(QGraphicsSceneDragDropEvent* event);
    bool canMove(int x, int y, QPair<int, int> size = {1, 1});
    AbstractDragItem* getNeighbour(QPoint coord);
    QPoint getNextCoord(QPoint coord);
    void updateCoord(QPoint coord, QPoint nextCoord);
    void updatePacmanByNeighbour(DragItemFabric::DragItemType type, QPoint nextCoord);
    void updateBoxAreaByNeighbour(DragItemFabric::DragItemType type, AbstractDragItem* neighbour);

private:
    qreal BOX_SIZE_ = 20;
    PacManItem* pacman_;
    QPoint startPos_;
    QPoint currentPos_;
    QPoint pacmanPos_;
    AbstractDragItem* currentDropItem_ = nullptr;
    QVector<AbstractDragItem*> items_;
    QMap<QPair<int, int>, AbstractDragItem*> coordToItemsMap_;
    QMultiMap<AbstractDragItem*, QPair<int, int>> itemsToCoordMap_;
    QVector<QVector<int>> boxArea_;

    QTimer* gameTimer_;

};

#endif // DROPPABLEAREA_H
