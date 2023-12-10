#include "itemswidget.h"

#include <QByteArray>
#include <QDataStream>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>

ItemsWidget::ItemsWidget(QWidget *parent) : QListWidget(parent) {
  setSelectionMode(SingleSelection);
  setDragEnabled(true);
}

void ItemsWidget::mousePressEvent(QMouseEvent *event) {
  dragStartPosition_ = event->pos();
  dragableItem_ = dynamic_cast<DragItem *>(itemAt(dragStartPosition_));

  QListWidget::mousePressEvent(event);
}

void ItemsWidget::mouseMoveEvent(QMouseEvent *event) {
  if ((event->buttons() == Qt::LeftButton) == false ||
      dragableItem_ == nullptr) {
    return;
  }

  QByteArray itemData;
  QDataStream dataStream(&itemData, QIODevice::WriteOnly);
  dataStream << dragableItem_->GetType();

  QMimeData *mimeData = new QMimeData;
  mimeData->setData(dragableItem_->kMimeFormat, itemData);

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->setPixmap(GetPreview());

  drag->exec(Qt::CopyAction | Qt::MoveAction);
}

QPixmap ItemsWidget::GetPreview() const {
  auto *item = dragableItem_->GetGraphicsItem();

  QPixmap pixmap(item->boundingRect().size().toSize());
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setPen(item->pen());
  painter.setBrush(item->brush());
  painter.drawPath(item->shape());

  return pixmap;
}
