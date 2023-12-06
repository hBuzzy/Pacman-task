#include "pacmanitem.h"

#include <QPainter>

PacmanItem::PacmanItem(QPainterPath geometry, QPen pen, QBrush brush)
  : QAbstractGraphicsShapeItem(),
    geometry_(geometry),
    selectedPenFront_(pen),
    selectedPenBack_(pen) {

  setPen(pen);
  setBrush(brush);

  coordinateX_ = 1;
  coordinateY_ = 0;
  step_ = 1;

  selectedPenBack_ = QPen(kSelectedPenColor_, pen.width(), Qt::SolidLine);
  selectedPenFront_ = QPen(pen.color(), pen.width(), kSelectedPenStyle_);

  setFlags(ItemIsMovable | ItemIsSelectable);
}

QPainterPath PacmanItem::shape() const { return geometry_; }

QRectF PacmanItem::boundingRect() const {
  int divider = 2;
  qreal penThickness = pen().widthF();

  return geometry_.boundingRect().adjusted(-penThickness / divider,
                                           -penThickness / divider,
                                           penThickness / divider,
                                           penThickness / divider);
}

void PacmanItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setRenderHint(QPainter::Antialiasing);

  painter->setPen(pen());
  painter->setBrush(brush());
  painter->drawPath(geometry_);

  if (isSelected()) {
    painter->setPen(selectedPenBack_);
    DrawBorder(painter);

    painter->setPen(selectedPenFront_);
    DrawBorder(painter);
  }
}

uint PacmanItem::getCoordinateX() { return coordinateX_; }

uint PacmanItem::getCoordinateY() { return coordinateY_; }

uint PacmanItem::getStep() { return step_; }

void PacmanItem::setCoordinates(uint x, uint y) {
  coordinateX_ = x;
  coordinateY_ = y;
}

void PacmanItem::movePlayer() {
  int newCoordinateX = pos().x() + coordinateX_ * step_;
  int newCoordinateY = pos().y() + coordinateY_ * step_;
  setPos(newCoordinateX, newCoordinateY);
}

void PacmanItem::invertCoordinates() {
  coordinateX_ = -coordinateX_;
  coordinateY_ = -coordinateY_;
}

void PacmanItem::DrawBorder(QPainter *painter) { painter->drawPath(geometry_); }
