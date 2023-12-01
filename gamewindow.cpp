#include "gamewindow.h"

GameWindow::GameWindow(QVector<QVector<int>> boxArea, QWidget *parent) : QWidget (parent) {
    boxArea_ = boxArea;
    initializeGame();
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateGame);
    timer->start(500);
}

void GameWindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    drawGameArea(&painter);
    pacman_->paint(&painter, nullptr, nullptr);
}

void GameWindow::initializeGame() {

}

void GameWindow::drawGameArea(QPainter* painter) {

}

void GameWindow::updateGame() {
    movePacman();
    checkCollision();
    update();
}

void GameWindow::movePacman() {
}

void GameWindow::checkCollision() {

}
