#ifndef DRAGITEM_H
#define DRAGITEM_H

#include <itemsfactory.h>

#include <QGraphicsItem>
#include <QListWidget>
#include <QWidget>

class DragItem : public QWidget, public QListWidgetItem {
  Q_OBJECT
 public:
  explicit DragItem(ItemsFactory::ItemsType type, QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event);

 public:
  PacmanItem *GetGraphicsItem() const;
  ItemsFactory::ItemsType GetType() const;

 public:
  static const inline QString kMimeFormat = "application/x-dnditemdata";

 private:
  PacmanItem *graphicsItem_;
  ItemsFactory::ItemsType type_;
};

#endif // DRAGITEM_H
