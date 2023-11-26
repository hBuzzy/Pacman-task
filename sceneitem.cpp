#include "sceneitem.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>

class ItemsFactory;

SceneItem::SceneItem(QPainterPath figure, QPen pen, QBrush brush)
    : QAbstractGraphicsShapeItem(),
      geometry_(figure),
      selectedPenFront_(pen),
      selectedPenBack_(pen),
      originalBrush_(brush),
      currentBrush_(brush) {
  setPen(pen);
  setBrush(brush);

  directionX_ = 1;
  directionY_ = 0;
  step_ = 1;

  selectedPenBack_ = QPen(kSelectedPenColor_, pen.width(), Qt::SolidLine);
  selectedPenFront_ = QPen(pen.color(), pen.width(), kSelectedPenStyle_);

  setFlags(ItemIsMovable | ItemIsSelectable);
}

void SceneItem::ChangeBrush(const QBrush &newBrush) {
  currentBrush_ = newBrush;
  setBrush(currentBrush_);
}

int SceneItem::GetDirectionX() { return directionX_; }

int SceneItem::GetDirectionY() { return directionY_; }

int SceneItem::GetStep() { return step_; }

QPainterPath SceneItem::shape() const { return geometry_; }

QRectF SceneItem::boundingRect() const {
  int divider = 2;
  qreal penThickness = pen().widthF();

  return geometry_.boundingRect().adjusted(
      -penThickness / divider, -penThickness / divider, penThickness / divider,
      penThickness / divider);
}

void SceneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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

void SceneItem::Move() {
  int newX = pos().x() + directionX_ * step_;
  int newY = pos().y() + directionY_ * step_;
  setPos(newX, newY);
}

void SceneItem::DrawBorder(QPainter *painter) { painter->drawPath(geometry_); }

void SceneItem::SetDirection(int x, int y) {
  directionX_ = x;
  directionY_ = y;
}

void SceneItem::InvertDirection() {
  directionX_ = -directionX_;
  directionY_ = -directionY_;
}
