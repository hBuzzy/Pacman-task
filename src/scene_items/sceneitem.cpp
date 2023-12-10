#include "sceneitem.h"

#include <QPainter>

SceneItem::SceneItem(QRectF rect, QPen pen, QBrush brush, QString type)
    : QAbstractGraphicsShapeItem() {
  geometry_ = rect;
  type_ = type;
  setPen(pen);
  setBrush(brush);
  selectedPenBack_ = QPen(kSelectedPenColor_, pen.width(), Qt::SolidLine);
  selectedPenFront_ = QPen(pen.color(), pen.width(), kSelectedPenStyle_);

  setFlags(ItemIsMovable | ItemIsSelectable);

  step_ = 10;
}

SceneItem::SceneItem(SceneItem const &sceneItem)
    : QAbstractGraphicsShapeItem() {
  SceneItem(sceneItem.boundingRect(), sceneItem.pen(), sceneItem.brush());
}

QPainterPath SceneItem::shape() {
  QPainterPath path;
  if (type_ == "Монетка") {
    path.addEllipse(boundingRect());
  } else if (type_ == "Игрок") {
    path.addPolygon(DrawPacmanShape());
  } else {
    path.addRect(boundingRect());
  }

  return path;
}

QRectF SceneItem::boundingRect() const {
  int divider = 2;
  qreal penThickness = pen().widthF();

  return QRectF(-penThickness / divider, -penThickness / divider,
                geometry_.width() + penThickness,
                geometry_.height() + penThickness);
}

void SceneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setRenderHint(QPainter::Antialiasing);

  painter->setPen(pen());
  painter->setBrush(brush());

  if (type_ == "Монетка") {
    painter->drawEllipse(geometry_);

  } else if (type_ == "Игрок") {
    painter->drawPolygon(DrawPacmanShape());
  } else {
    painter->drawRect(geometry_);
  }

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
void SceneItem::SetDirection(int x, int y) {
  directionX_ = x;
  directionY_ = y;
}
void SceneItem::InvertDirection() {
  directionX_ = -directionX_;
  directionY_ = -directionY_;
}

void SceneItem::DrawBorder(QPainter *painter) {
  painter->drawLine(geometry_.topLeft(), geometry_.topRight());
  painter->drawLine(geometry_.topLeft(), geometry_.bottomLeft());
  painter->drawLine(geometry_.bottomRight(), geometry_.topRight());
  painter->drawLine(geometry_.bottomRight(), geometry_.bottomLeft());
}

QPolygonF SceneItem::DrawPacmanShape() {
  int delimiter = 2;
  QPolygonF pacmanShape;
  float centerX = geometry_.width() / delimiter;
  float centerY = geometry_.height() / delimiter;
  float radius = geometry_.height() / delimiter;

  float angleStart = 60;
  float angleEnd = 300;
  pacmanShape << QPointF(centerX, centerY);

  for (int angle = angleStart; angle <= angleEnd; angle++) {
    float radians = angle * M_PI / 180.0f;
    float x = centerX + radius * std::cos(radians);
    float y = centerY + radius * std::sin(radians);
    pacmanShape << QPointF(x, y);
  }
  return pacmanShape;
}
int SceneItem::GetDirectionX() { return directionX_; }

int SceneItem::GetDirectionY() { return directionY_; }

int SceneItem::GetStep() { return step_; }

void SceneItem::ChangeBrush(const QBrush &newBrush) {
  currentBrush_ = newBrush;
  setBrush(currentBrush_);
}
