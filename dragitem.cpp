#include "dragitem.h"

#include <QPainter>
#include <QRect>

DragItem::DragItem(ItemsFactory::ItemsType type, QWidget *parent)
    : QWidget{parent}, QListWidgetItem() {
  graphicsItem_ = ItemsFactory::Create(type);
  type_ = type;

  QRect geometry = graphicsItem_->boundingRect().toRect();
  setGeometry(geometry);

  // Задаем размер элемента списка (QListWidgetItem)
  // Делаем его чуть больше в размере, чтобы элементы списка не слипались
  int additionalHeight = 2;
  setSizeHint(QSize(geometry.width(), geometry.height() + additionalHeight));
}

// Отрисовываем нашу фигуру в списке
void DragItem::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)

  QPainter painter(this);
  painter.setPen(graphicsItem_->pen());
  painter.setBrush(graphicsItem_->brush());

  QStyleOptionGraphicsItem itemOption;
  graphicsItem_->paint(&painter, &itemOption, nullptr);
}

PacmanItem *DragItem::GetGraphicsItem() const { return graphicsItem_; }

ItemsFactory::ItemsType DragItem::GetType() const { return type_; }
