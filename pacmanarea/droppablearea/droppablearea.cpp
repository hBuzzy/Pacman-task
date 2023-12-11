#include "droppablearea.h"
#include <QDebug>
#include <memory>


DroppableArea::DroppableArea(int x, int y, qreal width, qreal height) {
    setRect(x, y, width, height);
    startPos_ = {x, y};
    setAcceptDrops(true);
    boxArea_ = QVector<QVector<int>>(width / BOX_SIZE_, QVector<int>(height / BOX_SIZE_, 0));
    pacman_ = PacManItem::instance();
    gameTimer_ = new QTimer();
    pacmanPos_ = QPoint(-1, -1);

    connect(gameTimer_, &QTimer::timeout, this, &DroppableArea::updateGame);
}

void DroppableArea::initGame() {
    if (pacmanPos_.x() == -1) { // выводит true
        QMessageBox::information(nullptr, "Ошибка", "Pacman не пристуствует на поле");
        return;
    }
    setAcceptDrops(false);
    gameTimer_->start(500);
}

void DroppableArea::closeGame() {
    setAcceptDrops(true);
    gameTimer_->stop();
}

void DroppableArea::updateGame() {
    QPoint nextCoord = getNextCoord(pacmanPos_);
    int x = nextCoord.x();
    int y = nextCoord.y();
    if(x >= boxArea_.size() || y >= boxArea_[0].size() || x < 0 || y < 0) {
        pacman_->setDark();
    } else {
        AbstractDragItem* neighbour = getNeighbour(nextCoord);

        if(neighbour == nullptr){
            updateCoord(pacmanPos_, nextCoord);
            pacman_->setGreen();
        } else {
            updatePacmanByNeighbour(DragItemFabric::getTypeByName(neighbour->getName()), nextCoord);
            updateBoxAreaByNeighbour(DragItemFabric::getTypeByName(neighbour->getName()), neighbour);
            neighbour->touch();
        }
    }
    scene()->removeItem(pacman_);
    scene()->addItem(pacman_);
}

void DroppableArea::updateCoord(QPoint coord, QPoint nextCoord){
    int x = nextCoord.x();
    int y = nextCoord.y();

    boxArea_[coord.x()][coord.y()] = 0;
    boxArea_[x][y] = PacManItem::value();
    pacmanPos_.setX(x);
    pacmanPos_.setY(y);
    pacman_->setCoord(x * BOX_SIZE_ + startPos_.x(), y * BOX_SIZE_ + startPos_.y());
    qDebug() << pacmanPos_;
};

void DroppableArea::updatePacmanByNeighbour(DragItemFabric::DragItemType type, QPoint nextCoord){
    switch(type){
        case DragItemFabric::COIN:
            updateCoord(pacmanPos_, nextCoord);
            emit updateScore();
            break;
        case DragItemFabric::VWALL:
        case DragItemFabric::HWALL:
            pacman_->setDark();
            break;
    }
};

void DroppableArea::updateBoxAreaByNeighbour(DragItemFabric::DragItemType type, AbstractDragItem* neighbour){
    switch(type){
        case DragItemFabric::COIN:
            while (coordToItemsMap_.values().contains(neighbour)) {
                auto point = itemsToCoordMap_.find(neighbour).value();
                boxArea_[point.first][point.second] = 0;
                coordToItemsMap_.remove(point);
                itemsToCoordMap_.remove(neighbour, point);
            }
            scene()->removeItem(neighbour);
            break;
        case DragItemFabric::VWALL:
        case DragItemFabric::HWALL:
            break;
    }
};

AbstractDragItem* DroppableArea::getNeighbour(QPoint coord){
    if (coordToItemsMap_.contains({coord.x(), coord.y()}) )
        return coordToItemsMap_.find({coord.x(), coord.y()}).value();
    else return nullptr;
};

QPoint DroppableArea::getNextCoord(QPoint coord){
    switch(pacman_->direction()){
        case PacManItem::DOWN:
            return {pacmanPos_.x(), pacmanPos_.y() + 1};
        case PacManItem::UP:
            return {pacmanPos_.x(), pacmanPos_.y() - 1};
        case PacManItem::LEFT:
            return {pacmanPos_.x() - 1, pacmanPos_.y()};
        case PacManItem::RIGHT:
            return {pacmanPos_.x() + 1, pacmanPos_.y()};
    }
};


void DroppableArea::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText()) {
        auto coord = getBoxCoord(event);
        int x = coord.x();
        int y = coord.y();
        if(x < boxArea_.size() && y < boxArea_[0].size() && x > 0 && y > 0){
            if (event->mimeData()->text() == PacManItem::name()) {
                boxArea_[x][y] = 0;
                event->setAccepted(true);
                return;
            }
            if(!coordToItemsMap_.contains({x, y})) {
                pacman_->setRadius(BOX_SIZE_);
                event->setAccepted(false);
                return;
            }
            AbstractDragItem* temp = coordToItemsMap_.find({x, y}).value();
            while (coordToItemsMap_.values().contains(temp)) {
                auto point = itemsToCoordMap_.find(temp).value();
                boxArea_[point.first][point.second] = 0;
                coordToItemsMap_.remove(point);
                itemsToCoordMap_.remove(temp, point);
            }
            currentDropItem_ = temp;
        }
        event->setAccepted(true);
    } else {
        event->setAccepted(false);
    }
}

void DroppableArea::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    auto coord = getBoxCoord(event);
    int x = coord.x();
    int y = coord.y();
    if (event->mimeData()->text() == PacManItem::name()) {
        if(x > boxArea_.size() - 1 || y > boxArea_[0].size() - 1  || x < 0 || y < 0 ) {
            return;
        }
        if(x < boxArea_.size() && y < boxArea_[0].size() && !canMove(x, y)){
            return;
        }
        pacman_->setCoord(x * BOX_SIZE_ + startPos_.x(), y * BOX_SIZE_ + startPos_.y());
        scene()->removeItem(pacman_);
        scene()->addItem(pacman_);
        return;
    }
    if(currentDropItem_ != nullptr){
        auto size = currentDropItem_->getSize();
        if(x > boxArea_.size() - size.first || y > boxArea_[0].size() - size.second  || x < 0 || y < 0 ) {
            return;
        }
        if(x < boxArea_.size() && y < boxArea_[0].size() && !canMove(x, y, size)){
            return;
        }
        scene()->removeItem(currentDropItem_);
    }
    auto type = DragItemFabric::getTypeByName(event->mimeData()->text());
    currentDropItem_ =  DragItemFabric::getItemByType(type, x * BOX_SIZE_ + startPos_.x(), y * BOX_SIZE_ + startPos_.y(), BOX_SIZE_);
    scene()->addItem(currentDropItem_);
    currentPos_ = {x, y};
}

void DroppableArea::dropEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText()) {
        if (event->mimeData()->text() == PacManItem::name()) {
            auto coord = getBoxCoord(event);
            boxArea_[coord.x()][coord.y()] = PacManItem::value();
            pacmanPos_.setX(coord.x());
            pacmanPos_.setY(coord.y());
            qDebug() << pacmanPos_;
            return;
        }
        auto type = DragItemFabric::getTypeByName(event->mimeData()->text());
        int x = currentPos_.x();
        int y = currentPos_.y();
        auto size = currentDropItem_->getSize();
        AbstractDragItem* temp = currentDropItem_;
        items_.append(temp);
        for (int i = 0; i < size.first; i++) {
            for(int j = 0; j < size.second; j++) {
                boxArea_[x + i][y + j] = DragItemFabric::getNumByType(type);
                coordToItemsMap_.insert({x + i, y + j}, temp);
                itemsToCoordMap_.insertMulti(temp, {x + i, y + j});
            }
        }
        currentDropItem_ = nullptr;
    }
}

bool DroppableArea::canMove(int x, int y, QPair<int, int> size) {
    for (int i = 0; i < size.first; i++) {
        for(int j = 0; j < size.second; j++) {
            if(boxArea_[x + i][y + j]){
                return false;
            }
        }
    }
    return true;
}

QPoint DroppableArea::getBoxCoord(QGraphicsSceneDragDropEvent* event) {
    qreal x = event->scenePos().x() - event->mimeData()->text().toDouble() - startPos_.x();
    qreal y = event->scenePos().y() - event->mimeData()->text().toDouble() - startPos_.y();
    return { qFloor(x / BOX_SIZE_), qFloor(y / BOX_SIZE_) };
}
