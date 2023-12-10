#ifndef ITEMSWIDGET_H
#define ITEMSWIDGET_H

#include <src/dragitem/dragitem.h>

#include <QListWidget>

class ItemsWidget : public QListWidget {
  Q_OBJECT
 public:
  explicit ItemsWidget(QWidget *parent = nullptr);

 protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

 private:
  QPixmap GetPreview() const;

 private:
  QPoint dragStartPosition_;
  DragItem *dragableItem_;
};

#endif  // ITEMSWIDGET_H
