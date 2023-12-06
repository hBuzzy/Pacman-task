#ifndef PACMANITEM_H
#define PACMANITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QPen>

class PacmanItem : public QAbstractGraphicsShapeItem {
 public:
  explicit PacmanItem(QPainterPath geometry, QPen pen, QBrush brush);

 public:
  QPainterPath shape() const;
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

  void setCoordinates(uint x, uint y);
  void movePlayer();

  uint getCoordinateX();
  uint getCoordinateY();
  uint getStep();
  void invertCoordinates();

 private:
  void DrawBorder(QPainter *painter);

 private:
  const QColor kSelectedPenColor_ = Qt::white;
  const Qt::PenStyle kSelectedPenStyle_ = Qt::DashLine;

  QPainterPath geometry_;
  QPen selectedPenFront_;
  QPen selectedPenBack_;

  uint coordinateX_;
  uint coordinateY_;
  uint step_;
};

#endif // PACMANITEM_H
