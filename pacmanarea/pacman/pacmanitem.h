#ifndef PACMANITEM_H
#define PACMANITEM_H

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QBrush>
#include <QPainter>

class PacManItem :public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum Direction {UP, DOWN, LEFT, RIGHT};
public:
    static PacManItem* instance(int x = 0, int y = 0, int radius = 0);
    static QString name();
    static int value();
    static Direction direction();
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void setCoord(int x, int y);
    QPoint getCoord();
    void setGreen();
    void setDark();
    void setRadius(int radius);

public slots:
    void keyPressEvent(QKeyEvent* event) override;

private:
    PacManItem(int x, int y, int radius);
    int radius_;
    int angle_;
    int pie_;
    int x_;
    int y_;
    QColor color_;
    static QString name_;
    static int value_;
    static PacManItem* instance_;
    static Direction direction_;
};

#endif // PACMANITEM_H
