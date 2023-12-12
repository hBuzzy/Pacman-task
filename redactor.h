#ifndef REDACTOR_H
#define REDACTOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QPushButton>

#include "mainwindow.h"
#include "customgraphicsview.h"

namespace Ui {
class Redactor;
}

class Redactor : public QMainWindow {
    Q_OBJECT

public:
    Redactor(QWidget *parent = nullptr);
    ~Redactor();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::Redactor *ui;
    MainWindow *MW;
    int **gameGrid_;
    const int rows_ = 10;
    const int cols_ = 10;
    int gridSize_ = 64;
    void setupGameGrid();
    QGraphicsScene *scene;
    CustomGraphicsView *view;
    bool isdrawing_ = false;
    QPointF center_;
    int lineWidth_;
    QColor lineColor_;
    void drawItems(QPainter *painter, const QPoint &center);
    int getElement();
    QGraphicsPixmapItem* wallItem_;
    QGraphicsPixmapItem* puckmanItem_;
    QGraphicsPixmapItem* hostileItem_;
    QGraphicsPixmapItem* coinItem_;
    QGraphicsPixmapItem* dragItem_ = nullptr;
    QPoint getGridPoint();
    int dragElement_;
    void dragItem();
    void handleCustomMouseRelease();
    bool isStatePacmanElement_ = false;
    QPushButton *myButton;
    void handleButtonClick();
    void openGame();
    void exitRedaction();

};

#endif // REDACTOR_H
