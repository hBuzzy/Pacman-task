#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QDebug>
#include <QPen>
#include <QtMath>

class SceneItem : public QAbstractGraphicsShapeItem {
 public:
  explicit SceneItem(QRectF rect, QPen pen, QBrush brush, QString type = "");
  SceneItem(SceneItem const &sceneItem);

 public:
  QPainterPath shape();
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);
  void Move();
  void SetDirection(int x, int y);
  void InvertDirection();
  void ChangeBrush(const QBrush &newBrush);
  int GetDirectionX();
  int GetDirectionY();
  int GetStep();

 private:
  void DrawBorder(QPainter *painter);
  QPolygonF DrawPacmanShape();

 private:
  const QColor kSelectedPenColor_ = Qt::white;
  const Qt::PenStyle kSelectedPenStyle_ = Qt::DashLine;
  QBrush originalBrush_;
  QBrush currentBrush_;
  QRectF geometry_;
  QPen selectedPenFront_;
  QPen selectedPenBack_;
  int directionX_;
  int directionY_;
  int step_;
  QString type_;
};

#endif  // SCENEITEM_H
