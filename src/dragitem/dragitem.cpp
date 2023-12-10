#include "dragitem.h"

#include <QPainter>
#include <QRect>

DragItem::DragItem(const QString &type, QWidget *parent)
    : QWidget{parent}, QListWidgetItem() {
  graphicsItem_ = ItemsFactory::Create(type);
  type_ = type;

  QRect geometry = graphicsItem_->boundingRect().toRect();
  setGeometry(geometry);
  int additionalHeight = 2;
  setSizeHint(QSize(geometry.width(), geometry.height() + additionalHeight));
}

void DragItem::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)

  QPainter painter(this);
  painter.setPen(graphicsItem_->pen());
  painter.setBrush(graphicsItem_->brush());

  QStyleOptionGraphicsItem itemOption;
  graphicsItem_->paint(&painter, &itemOption, nullptr);
}

SceneItem *DragItem::GetGraphicsItem() const { return graphicsItem_; }

QString DragItem::GetType() const { return type_; }
