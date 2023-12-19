#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CustomGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr, int gridSize = 64);

    void setCursorStyle();
    int getCenterX() const;
    int getCenterY() const;
    void setCenterX(int newX);
    void setCenterY(int newY);
    void setCenter(QPointF isDrawing);
    void setDrawing(bool newDrawing);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int cellSize_;
    QPointF center_;
    bool isDrawing_ = false;

signals:
    void customMouseRelease();
};

#endif // CUSTOMGRAPHICSVIEW_H
