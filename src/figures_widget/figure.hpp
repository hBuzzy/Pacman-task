#ifndef PACMAN_FIGURE_HPP
#define PACMAN_FIGURE_HPP

#include <QGraphicsItem>
#include <QListWidget>
#include <QWidget>
#include <QPainter>

#include "scene/objects/scene_object.hpp"

class Figure final : public QWidget, public QListWidgetItem {
    Q_OBJECT
public:
    explicit Figure(SceneObject* sceneObj, QWidget* parent = nullptr)
    : baseSceneObject_(sceneObj), QWidget(parent), QListWidgetItem() {
        setGeometry(sceneObj->shape().boundingRect().toRect());
        constexpr int listGap = 2;
        setSizeHint({
            static_cast<int>(sceneObj->PreviewShape().boundingRect().width()),
            static_cast<int>(sceneObj->PreviewShape().boundingRect().height()) + listGap
        });
        setText("\t" + names_[sceneObj->GetType()]);
    }
    ~Figure() final { delete baseSceneObject_; }
private:
    void paintEvent(QPaintEvent *event) final {
        Q_UNUSED(event)
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing);
        QStyleOptionGraphicsItem itemOption;
        baseSceneObject_->PaintPreview(&painter, &itemOption);
    }
public:
    SceneObject* GetSceneObject() { return baseSceneObject_; }
public:
    static constexpr auto kMimeFormat = "application/x-dnditemdata";
private:
    static inline std::unordered_map<SceneObject::Type, QString> names_ {
        {SceneObject::kWall, "Wall"},
        {SceneObject::kCoin, "Coin"},
        {SceneObject::kPacman, "Pacman"}
    };
    SceneObject* baseSceneObject_;
};

#endif
