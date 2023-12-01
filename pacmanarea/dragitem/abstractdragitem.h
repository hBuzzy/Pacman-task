#ifndef ABSTRACTDRAGITEM_H
#define ABSTRACTDRAGITEM_H

#include <QGraphicsRectItem>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QBrush>
#include <QPainter>

class AbstractDragItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    AbstractDragItem();
    AbstractDragItem(qreal x, qreal y, qreal boxSize, QString name, QColor color);
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    QPair<int, int> getSize();
    QString getName();

public slots:
    virtual void touch() = 0;

protected:
    int WIDTH_ = 1;
    int HEIGHT_ = 1;

private:
    QString name_;
};

#endif // ABSTRACTDRAGITEM_H
