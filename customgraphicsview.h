#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class CustomGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    CustomGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr, int gridSize = 64);
    QPointF center;
    bool isdrawing = false;
    void setCursorStyle();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int gridSize;

signals:
    void customMouseRelease();
};

#endif // CUSTOMGRAPHICSVIEW_H
