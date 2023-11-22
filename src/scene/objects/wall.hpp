#ifndef PACMAN_WALL_HPP
#define PACMAN_WALL_HPP

#include "figures_widget/figure.hpp"

struct box2 {
    static constexpr int maxSize = 10;
    int top, left, bottom, right;
public:
    box2& operator+=(const box2& rhs) noexcept {
        top = std::max(0, std::min(maxSize, top + rhs.top));
        left = std::max(0, std::min(maxSize, left + rhs.left));
        bottom = std::max(0, std::min(maxSize, bottom + rhs.bottom));
        right = std::max(0, std::min(maxSize, right + rhs.right));
        return *this;
    }
};

class Wall final : public SceneObject {
public:
    explicit Wall(const box2& stretching)
    : stretching_(stretching), SceneObject({{0, 0, 255}, Qt::SolidPattern}) {}
public:
    [[nodiscard]] QPainterPath shape() const final {
        QPainterPath path;
        path.addPolygon(QPolygonF{QList<QPointF>{
            {0, 0},
            QPointF{0, static_cast<qreal>(-stretching_.top)} * static_cast<qreal>(cellSize),
            QPointF{1, static_cast<qreal>(-stretching_.top)} * static_cast<qreal>(cellSize),
            QPointF{1, 0} * static_cast<qreal>(cellSize),
            QPointF{static_cast<qreal>(stretching_.right + 1), 0} * static_cast<qreal>(cellSize),
            QPointF{static_cast<qreal>(stretching_.right + 1), 1} * static_cast<qreal>(cellSize),
            QPointF{1, 1} * static_cast<qreal>(cellSize),
            QPointF{1, static_cast<qreal>(stretching_.bottom + 1)} * static_cast<qreal>(cellSize),
            QPointF{0, static_cast<qreal>(stretching_.bottom + 1)} * static_cast<qreal>(cellSize),
            QPointF{0, 1} * static_cast<qreal>(cellSize),
            QPointF{static_cast<qreal>(-stretching_.left), 1} * static_cast<qreal>(cellSize),
            QPointF{static_cast<qreal>(-stretching_.left), 0} * static_cast<qreal>(cellSize),
        }});
        return path;
    }
    void AdjustStretching(const box2& ds) noexcept {
        stretching_ += ds;
    }
    [[nodiscard]] SceneObject::Type GetType() const final {
        return SceneObject::Type::kWall;
    };
    [[nodiscard]] static Wall* getInstance() {
        return new Wall({0, 0, 0, 0});
    };
    [[nodiscard]] static Figure* getFigure() {
        return new Figure(Wall::getInstance());
    }
private:
    box2 stretching_;
};

#endif
