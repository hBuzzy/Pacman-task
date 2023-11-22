#ifndef FIGURES_LIST_HPP
#define FIGURES_LIST_HPP

#include <QListWidget>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

#include "figures_widget/figure.hpp"

class FiguresList final : public QListWidget {
    Q_OBJECT
public:
    explicit FiguresList(QWidget* parent = nullptr)
    : QListWidget(parent) {
        setSelectionMode(SingleSelection);
        setDragEnabled(true);
    }
private:
    void mousePressEvent(QMouseEvent* event) final {
        dragStartPos_ = event->pos();
        draggedItem_ = dynamic_cast<Figure*>(itemAt(dragStartPos_));
        QListWidget::mousePressEvent(event);
    }
    void mouseMoveEvent(QMouseEvent* event) final {
        if (event->buttons() != Qt::LeftButton || draggedItem_ == nullptr)
            return;
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        auto* so = draggedItem_->GetSceneObject();
        dataStream << so->GetType();
        auto* mimeData = new QMimeData;
        mimeData->setData(Figure::kMimeFormat, itemData);
        QDrag drag{this};
        drag.setMimeData(mimeData);
        QPixmap pixmap(so->boundingRect().size().toSize());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        so->paint(&painter, nullptr, nullptr);
        drag.setPixmap(pixmap);
        drag.exec(Qt::CopyAction | Qt::MoveAction);
    }
private:
    QPoint dragStartPos_;
    Figure* draggedItem_ = nullptr;
};

#endif
