#include "pacmanitem.h"
#include <QDebug>

PacManItem* PacManItem::instance_ = nullptr;
QString PacManItem::name_ = "Pacman";
PacManItem::Direction PacManItem::direction_ = RIGHT;
int PacManItem::value_ = 10;

PacManItem::PacManItem(int x, int y, int radius) : radius_(radius), angle_(45), pie_(270), x_(x), y_(y) {
    setGreen();
};

QRectF PacManItem::boundingRect() const{
    return QRectF(x_, y_, 2 * radius_, 2 * radius_);
}

PacManItem* PacManItem::instance(int x, int y, int radius) {
    if (!instance_) {
        instance_ = new PacManItem(x, y, radius);
    }
    return instance_;
}

QString PacManItem::name() {
    return name_;
}

PacManItem::Direction PacManItem::direction() {
    return direction_;
}

int PacManItem::value() {
    return value_;
}

void PacManItem::setCoord(int x, int y) {
    x_= x;
    y_ = y;
    update();
}

QPoint PacManItem::getCoord(){return {x_, y_};};

void PacManItem::setRadius(int radius) {
    radius_ = radius;
    update();
}

void PacManItem::setGreen(){ color_ = Qt::green; };
void PacManItem::setDark(){ color_ = Qt::darkGreen; };

void PacManItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QMimeData* mimeData = new QMimeData;
        mimeData->setText(name_);

        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);

        drag->exec(Qt::MoveAction);
    }

    QGraphicsItem::mousePressEvent(event);
}

void PacManItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(color_);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rect(x_, y_, 2 * radius_, 2 * radius_);
    painter->drawPie(rect, angle_ * 16, pie_ * 16);
}

void PacManItem::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left:
            angle_ = 225;
            pie_ = 270;
            direction_ = LEFT;
            break;
        case Qt::Key_Right:
            angle_ = 45;
            pie_ = 270;
            direction_ = RIGHT;
            break;
        case Qt::Key_Up:
            angle_ = 135;
            pie_ = 270;
            direction_ = UP;
            break;
        case Qt::Key_Down:
            angle_ = 315;
            pie_ = 270;
            direction_ = DOWN;
            break;
    }
    update();
}
