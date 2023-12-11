#include "pacmanarea.h"

PacmanArea::PacmanArea(QWidget* parent) : QGraphicsView(parent) {

    scene_ = new QGraphicsScene(this);

    AbstractDragItem * vwall = DragItemFabric::getVerticalWall(
                    DEMO_BOX_SIZE_,
                    DEMO_BOX_SIZE_,
                    DEMO_BOX_SIZE_
                );
    AbstractDragItem * hwall = DragItemFabric::getHorisontalWall(
                    DEMO_BOX_SIZE_ * PRINT_ITEM_STEP,
                    DEMO_BOX_SIZE_,
                    DEMO_BOX_SIZE_
                );
    AbstractDragItem * coin = DragItemFabric::getCoin(
                    DEMO_BOX_SIZE_ * PRINT_ITEM_STEP * 2,
                    DEMO_BOX_SIZE_,
                    DEMO_BOX_SIZE_
                );
    PacManItem* pacMan = PacManItem::instance(
                    DEMO_BOX_SIZE_ * PRINT_ITEM_STEP * 3,
                    DEMO_BOX_SIZE_,
                    DEMO_BOX_SIZE_/2
                );

    scoreRect_ = std::make_unique<QGraphicsTextItem>();
    scoreRect_.get()->setPos(
                    DEMO_BOX_SIZE_ * PRINT_ITEM_STEP * 4,
                    DEMO_BOX_SIZE_
                );
    scoreRect_->setFont(QFont("Helvetica", 30, 5));

    scene_->addItem(scoreRect_.get());
    updateScore();
    scene_->addItem(vwall);
    scene_->addItem(hwall);
    scene_->addItem(coin);
    scene_->addItem(pacMan);

    droppableArea_ = new DroppableArea(
                    0,
                    ITEMS_FIELD_BOX_HEIGHT_COUNT_ * DEMO_BOX_SIZE_,
                    AREA_BOX_COUNT_ * DEMO_BOX_SIZE_,
                    AREA_BOX_COUNT_ * DEMO_BOX_SIZE_
                );

    scene_->addItem(droppableArea_);

    setScene(scene_);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFixedSize(AREA_BOX_COUNT_ * DEMO_BOX_SIZE_, (AREA_BOX_COUNT_ + ITEMS_FIELD_BOX_HEIGHT_COUNT_) * DEMO_BOX_SIZE_);

    connect(this, &PacmanArea::startGame, droppableArea_, &DroppableArea::initGame);
    connect(this, &PacmanArea::stopGame, droppableArea_, &DroppableArea::closeGame);
    connect(droppableArea_, &DroppableArea::updateScore, this, &PacmanArea::updateScore);
}

void PacmanArea::updateScore() {
    scoreRect_.get()->setPlainText(QString("Очки: %1").arg(score_++));
    scene_->removeItem(scoreRect_.get());
    scene_->addItem(scoreRect_.get());
}

void PacmanArea::keyPressEvent(QKeyEvent* event) {
    PacManItem::instance()->keyPressEvent(event);
};
