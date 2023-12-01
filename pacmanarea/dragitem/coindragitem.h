#ifndef COINDRAGITEM_H
#define COINDRAGITEM_H

#include "abstractdragitem.h"
#include "dragitemfabric.h"

class CoinDragItem : public AbstractDragItem
{
public:
    CoinDragItem(qreal x, qreal y, qreal boxSize);

public slots:
    void touch() override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    int WIDTH_ = 1;
    int HEIGHT_ = 1;
};

#endif // COINDRAGITEM_H
