#ifndef PACMANAREA_H
#define PACMANAREA_H

#include <QGraphicsView>
#include <QVector>
#include <QObject>
#include "./droppablearea/droppablearea.h"

class PacmanArea : public QGraphicsView {
    Q_OBJECT
public:
    PacmanArea(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent* event);

public slots:
    void updateScore();

signals:
    void startGame();
    void stopGame();

private:
    QGraphicsScene* scene_;
    DroppableArea* droppableArea_;
    const int DEMO_BOX_SIZE_ = 20;
    const int AREA_BOX_COUNT_ = 30;
    const int ITEMS_FIELD_BOX_HEIGHT_COUNT_ = 5;
    const int PRINT_ITEM_STEP = 5;

    int score_ = 0;
    std::unique_ptr<QGraphicsTextItem> scoreRect_;
};

#endif // PACMANAREA_H
