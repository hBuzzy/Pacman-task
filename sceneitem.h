#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <itemsfactory.h>

#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QPen>
class SceneItem : public QAbstractGraphicsShapeItem {
 public:
  explicit SceneItem(QPainterPath path, QPen pen, QBrush brush);
  SceneItem(const SceneItem &sceneItem);

 public:
  QPainterPath shape() const;
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

 private:
  const QColor kSelectedPenColor_ = Qt::white;
  const Qt::PenStyle kSelectedPenStyle_ = Qt::DashLine;

  QPainterPath geometry_;
  QPen selectedPenFront_;
  QPen selectedPenBack_;
  QBrush originalBrush_;
  QBrush currentBrush_;

  int directionX_;
  int directionY_;
  int step_;
};

#endif  // SCENEITEM_H
