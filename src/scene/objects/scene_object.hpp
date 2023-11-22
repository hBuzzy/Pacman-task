#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include <QAbstractGraphicsShapeItem>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>

class SceneObject : public QAbstractGraphicsShapeItem {
public:
    static constexpr int cellSize = 20;
    enum Type { kWall = 0, kCoin = 1, kPacman = 2 };
    static inline std::unordered_map<Type, std::function<SceneObject*()>> factories;
public:
    explicit SceneObject(const QBrush& brush = {{0, 0, 0}, Qt::SolidPattern})
    : QAbstractGraphicsShapeItem() {
        setBrush(brush);
        setPen({
            {{0, 0, 0}, Qt::SolidPattern}, 1,
            Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin
        });
        setFlags(ItemIsMovable | ItemIsSelectable);
    }
public:
    [[nodiscard]] virtual QPainterPath PreviewShape() const {
        return shape();
    };
    [[nodiscard]] QRectF boundingRect() const final {
        const QRectF bRect = shape().boundingRect();
        const qreal displacement = pen().widthF();
        return {
            bRect.x() - displacement, bRect.y() - displacement,
            bRect.width() + displacement, bRect.height() + displacement
        };
    }
    void paint(
        QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget
    ) final {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        painter->setPen(pen());
        painter->setBrush(brush());
        painter->drawPath(shape());
        if (isSelected()) {
            painter->strokePath(shape(), QPen({255, 0, 0}));
            QPainterPath bounds;
            bounds.addRect(boundingRect());
            painter->strokePath(bounds, QPen({255, 0, 0}));
        }
    }
    void PaintPreview(
        QPainter* painter,
        const QStyleOptionGraphicsItem* option
    ) {
        Q_UNUSED(option)
        painter->setPen(pen());
        painter->setBrush(brush());
        painter->drawPath(PreviewShape());
    }
    [[nodiscard]] virtual Type GetType() const = 0;
    static auto TypeEquals(Type type) {
        return [type](QGraphicsItem* item){
            return dynamic_cast<SceneObject*>(item)->GetType() == type;
        };
    };
    static SceneObject* Factory(Type type) {
        return factories[type]();
    };
};

#endif
