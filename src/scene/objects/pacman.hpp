#ifndef PACMAN_PACMAN_HPP
#define PACMAN_PACMAN_HPP

#include "figures_widget/figure.hpp"
#include "coin.hpp"

class Pacman final : public SceneObject {
public:
    enum Direction { kTop = 0, kLeft = 1, kBottom = 2, kRight = 3 };
public:
    Pacman(): SceneObject({normalColor_}) {};
public:
    [[nodiscard]] QPainterPath PreviewShape() const final {
        QPainterPath path = shape();
        path.translate(
            static_cast<qreal>(cellSize) / 2,
            static_cast<qreal>(cellSize) / 2 + pen().widthF()
        );
        return path;
    }
    [[nodiscard]] QPainterPath shape() const final {
        QPainterPath path;
        QGraphicsEllipseItem ellipse{
            static_cast<qreal>(-cellSize) / 2,
            static_cast<qreal>(-cellSize) / 2,
            cellSize, cellSize
        };
        if (mouthOpened_) {
            ellipse.setStartAngle(500);
            ellipse.setSpanAngle(4500);
        }
        path.addPath(ellipse.shape());
        return path;
    }
    [[nodiscard]] SceneObject::Type GetType() const final {
        return SceneObject::Type::kPacman;
    };
    [[nodiscard]] static Pacman* getInstance() {
        return new Pacman;
    };
    [[nodiscard]] static Figure* getFigure() {
        return new Figure(Pacman::getInstance());
    }
    void SetDirection(Direction direction) {
        direction_ = direction;
        switch (direction_) {
            case kTop:
                setRotation(270);
                break;
            case kLeft:
                setRotation(180);
                break;
            case kBottom:
                setRotation(90);
                break;
            case kRight:
                setRotation(0);
                break;
        }
    }
    void OpenMouth() noexcept {
        mouthOpened_ = true;
    }
    [[nodiscard]] Coin* Move() {
        mouthOpened_ = !mouthOpened_;
        int dx, dy;
        constexpr int step = cellSize / 2;
        switch (direction_) {
            case kTop:
                dx = 0, dy = -step;
                break;
            case kLeft:
                dx = -step, dy = 0;
                break;
            case kBottom:
                dx = 0, dy = step;
                break;
            case kRight:
                dx = step, dy = 0;
                break;
        }
        moveBy(dx, dy);
        auto collidingItms = collidingItems();
        if (
            !scene()->sceneRect().contains(sceneBoundingRect())
            || std::ranges::any_of(collidingItms, SceneObject::TypeEquals(kWall))
        ) {
            moveBy(-dx, -dy);
            setBrush({collisionColor_});
        } else
            setBrush({normalColor_});
        if (auto coin = std::ranges::find_if(
            collidingItms, SceneObject::TypeEquals(kCoin)
        ); coin != collidingItms.end())
            return dynamic_cast<Coin*>(*coin);
        return nullptr;
    }
private:
    static constexpr QColor normalColor_ {255, 255, 0};
    static constexpr QColor collisionColor_ {255, 0, 0};
    Direction direction_ = kRight;
    bool mouthOpened_ = true;
};

#endif
