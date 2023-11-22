#ifndef COIN_HPP
#define COIN_HPP

#include "figures_widget/figure.hpp"

class Coin final : public SceneObject {
public:
    Coin(): SceneObject({{255, 255, 0}, Qt::SolidPattern}) {};
public:
    [[nodiscard]] QPainterPath PreviewShape() const final {
        QPainterPath path;
        path.addEllipse(0, pen().widthF(), cellSize, cellSize);
        return path;
    }
    [[nodiscard]] QPainterPath shape() const final {
        QPainterPath path;
        path.addEllipse(
            0, 0, static_cast<qreal>(cellSize) / 2,
            static_cast<qreal>(cellSize) / 2);
        return path;
    }
    [[nodiscard]] SceneObject::Type GetType() const final {
        return SceneObject::Type::kCoin;
    };
    [[nodiscard]] static Coin* getInstance() {
        return new Coin;
    };
    [[nodiscard]] static Figure* getFigure() {
        return new Figure(Coin::getInstance());
    }
};


#endif
