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
        dragStartPosition_ = event->pos();
        draggedItem_ = dynamic_cast<Figure*>(itemAt(dragStartPosition_));
        QListWidget::mousePressEvent(event);
    }
    void mouseMoveEvent(QMouseEvent* event) final {
        if (event->buttons() != Qt::LeftButton || draggedItem_ == nullptr)
            return;
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        auto* sceneObject = draggedItem_->GetSceneObject();
        dataStream << sceneObject->GetType();
        auto* mimeData = new QMimeData;
        mimeData->setData(Figure::kMimeFormat, itemData);
        QDrag drag{this};
        drag.setMimeData(mimeData);
        QPixmap pixmap(sceneObject->boundingRect().size().toSize());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        sceneObject->paint(&painter, nullptr, nullptr);
        drag.setPixmap(pixmap);
        drag.exec(Qt::CopyAction | Qt::MoveAction);
    }
private:
    QPoint dragStartPosition_;
    Figure* draggedItem_ = nullptr;
};

#endif
