#include "dragitemfabric.h"

QString DragItemFabric::getNameByType(DragItemType type) {
    switch(type) {
        case VWALL:
            return "VWALL";
        case HWALL:
            return "HWALL";
        case COIN:
            return "COIN";
        default:
            throw std::runtime_error(QString("invalid DragItemType: %1").arg(type).toStdString());
    }
};

DragItemFabric::DragItemType DragItemFabric::getTypeByName(QString name) {
    if (name == "VWALL") {
        return VWALL;
    } else if (name == "HWALL") {
        return HWALL;
    } else if (name == "COIN") {
        return COIN;
    }
    throw std::runtime_error(QString("invalid name: %1 for get DragItemType").arg(name).toStdString());
};

int DragItemFabric::getNumByType(DragItemType type) {
    switch(type) {
        case VWALL:
            return 1;
        case HWALL:
            return 2;
        case COIN:
            return 3;
        default:
            return 0;
    }
};

DragItemFabric::DragItemType DragItemFabric::getTypeByNum(int value) {
    switch(value) {
        case 1:
            return VWALL;
        case 2:
            return HWALL;
        case 3:
            return COIN;
        default:
            throw std::runtime_error(QString("invalid value: %1 for get DragItemType").arg(value).toStdString());
    }
};

AbstractDragItem* DragItemFabric::getVerticalWall(qreal x, qreal y, qreal boxSize) {
    return new WallDragItem(x, y, boxSize, WallDragItem::VERTICAL);
};

AbstractDragItem* DragItemFabric::getCoin(qreal x, qreal y, qreal boxSize) {
    return new CoinDragItem(x, y, boxSize);
};

AbstractDragItem* DragItemFabric::getHorisontalWall(qreal x, qreal y, qreal boxSize) {
    return new WallDragItem(x, y, boxSize, WallDragItem::HORISONTAL);
};

AbstractDragItem* DragItemFabric::getItemByType(DragItemType type, qreal x, qreal y, qreal boxSize) {
    switch(type) {
        case VWALL:
            return getVerticalWall(x, y, boxSize);
        case HWALL:
            return getHorisontalWall(x, y, boxSize);
        case COIN:
            return getCoin(x, y, boxSize);
        default:
            throw std::runtime_error(QString("invalid DragItemType: %1 for get Item").arg(type).toStdString());
    }
};
