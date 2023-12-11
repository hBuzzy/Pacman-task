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
    const int kDemoBoxSize = 20;
    const int kAreaBoxCount = 30;
    const int ItemsFieldBoxHeightCount = 5;
    const int PrintItemStep = 5;

    int score_ = 0;
    std::unique_ptr<QGraphicsTextItem> scoreRect_;
};

#endif // PACMANAREA_H
