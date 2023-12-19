#ifndef REDACTOR_H
#define REDACTOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QPushButton>

#include "mainwindow.h"
#include "customgraphicsview.h"

namespace Ui
{
    class Redactor;
}

class Redactor : public QMainWindow
{
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
    const int kRows_ = 10;
    const int kColumns_ = 10;
    int cellSize_ = 64;
    int counsCount_ = 0;
    QGraphicsScene *scene;
    CustomGraphicsView *view;
    bool isDrawing_ = false;
    QPointF center_;
    int lineWidth_;
    QColor lineColor_;
    int dragElement_;
    QGraphicsPixmapItem* wallItem_;
    QGraphicsPixmapItem* puckmanItem_;
    QGraphicsPixmapItem* hostileItem_;
    QGraphicsPixmapItem* coinItem_;
    QGraphicsPixmapItem* dragItem_ = nullptr;
    bool isStatePacmanElement_ = false;
    QPushButton *myButton;

    void setupGameGrid();
    void drawItems(QPainter *painter, const QPoint &center);
    QPoint getGridPoint();
    void dragItem();
    void handleCustomMouseRelease();
    void handleButtonClick();
    void openGame();
    void exitRedaction();
    int getElement();
};

#endif // REDACTOR_H
