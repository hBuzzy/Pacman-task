#include "redactor.h"
#include "ui_redactor.h"

#include <iostream>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

Redactor::Redactor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Redactor)
{
    setMouseTracking(true);
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    view->setMouseTracking(true);

    int uiWidth = (cols_+6) * gridSize_;
    int uiHeight = (rows_+1) * gridSize_;

    this->setFixedSize(uiWidth, uiHeight);
    setCentralWidget(view);

    gameGrid_ = new int*[rows_];
    for (int i = 0; i < rows_; ++i) {
        gameGrid_[i] = new int[cols_];
    }
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            gameGrid_[i][j] = 0;
        }
    }

    setupGameGrid();

    ui->setupUi(this);
}

Redactor::~Redactor()
{
    for (int i = 0; i < rows_; ++i) {
        delete[] gameGrid_[i];
    }
    delete[] gameGrid_;
    delete ui;
}

void Redactor::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    dragItem();
}

void Redactor::dragItem() {
    dragItem_ = new QGraphicsPixmapItem();
    if(dragElement_ == 1)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/wall.png").scaled(gridSize_, gridSize_));
    }
    if(dragElement_ == 2)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/puckman.png").scaled(gridSize_, gridSize_));
    }
    if(dragElement_ == 3)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/hostile.png").scaled(gridSize_, gridSize_));
    }
    if(dragElement_ == 4)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/coin.png").scaled(gridSize_, gridSize_));
    }
    dragItem_->setX(center_.x());
    dragItem_->setY(center_.y());
}

void Redactor::setupGameGrid() {
    // Очистите сцену перед обновлением
        scene->clear();
        QGraphicsPixmapItem* puckmanItem;
        QGraphicsPixmapItem* hostileItem;
        QGraphicsPixmapItem* coinItem;

        // Заполните сцену значениями из игровой сетки
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                switch (gameGrid_[i][j]) {
                case 0: // Empty space
                    scene->addRect(j * gridSize_, i * gridSize_, gridSize_, gridSize_, QPen(Qt::black), QBrush(Qt::white));
                    break;
                case 1: // Wall
                    scene->addRect(j * gridSize_, i * gridSize_, gridSize_, gridSize_, QPen(Qt::black), QBrush(Qt::darkGray));
                    break;
                case 2: // player
                    puckmanItem = scene->addPixmap(QPixmap(":resource/puckman.png").scaled(gridSize_, gridSize_));
                    puckmanItem->setX(j * gridSize_);
                    puckmanItem->setY(i * gridSize_);
                    break;
                case 3: // hostile
                    hostileItem = scene->addPixmap(QPixmap(":resource/hostile.png").scaled(gridSize_, gridSize_));
                    hostileItem->setX(j * gridSize_);
                    hostileItem->setY(i * gridSize_);
                    break;
                case 4: // hostile
                    coinItem = scene->addPixmap(QPixmap(":resource/coin.png").scaled(gridSize_, gridSize_));
                    coinItem->setX(j * gridSize_);
                    coinItem->setY(i * gridSize_);
                    break;
                }
            }
        }
        int RETURN = 2;

        wallItem_ = scene->addPixmap(QPixmap(":resource/wall.png").scaled(gridSize_, gridSize_));
        wallItem_->setX((cols_+RETURN) * gridSize_);
        wallItem_->setY(1 * gridSize_);
        qDebug() << "wallItem Coordinates: (" << wallItem_->x() << ", " << wallItem_->y() << ")";

        puckmanItem_ = scene->addPixmap(QPixmap(":resource/puckman.png").scaled(gridSize_, gridSize_));
        puckmanItem_->setX((cols_+RETURN) * gridSize_);
        puckmanItem_->setY(4 * gridSize_);
        qDebug() << "puckmanItem Coordinates: (" << puckmanItem_->x() << ", " << puckmanItem_->y() << ")";

        hostileItem_ = scene->addPixmap(QPixmap(":resource/hostile.png").scaled(gridSize_, gridSize_));
        hostileItem_->setX((cols_+RETURN) * gridSize_);
        hostileItem_->setY(6 * gridSize_);
        qDebug() << "hostileItem Coordinates: (" << hostileItem_->x() << ", " << hostileItem_->y() << ")";

        coinItem_ = scene->addPixmap(QPixmap(":resource/coin.png").scaled(gridSize_, gridSize_));
        coinItem_->setX((cols_+RETURN) * gridSize_);
        coinItem_->setY(8 * gridSize_);
        qDebug() << "coinItem Coordinates: (" << coinItem_->x() << ", " << coinItem_->y() << ")";

        // Установите размер сцены
        view->setSceneRect(0, 0, cols_ * gridSize_, rows_ * gridSize_);

        // Перерисовать сцену
        view->update();
        view->setMouseTracking(true);
}

int Redactor::getElement() {
    QGraphicsItem* item = scene->itemAt(center_.x(), center_.y(), QTransform());
    if(center_.x() < wallItem_->x()+gridSize_ && center_.x() > wallItem_->x()-gridSize_)
    {
        if(center_.y() < wallItem_->y()+gridSize_ && center_.y() > wallItem_->y()-gridSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return 1;
        }
    }
    if(center_.x() < puckmanItem_->x()+gridSize_ && center_.x() > puckmanItem_->x()-gridSize_)
    {
        if(center_.y() < puckmanItem_->y()+gridSize_ && center_.y() > puckmanItem_->y()-gridSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return 2;
        }
    }
    if(center_.x() < hostileItem_->x()+gridSize_ && center_.x() > hostileItem_->x()-gridSize_)
    {
        if(center_.y() < hostileItem_->y()+gridSize_ && center_.y() > hostileItem_->y()-gridSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return 3;
        }
    }
    if(center_.x() < coinItem_->x()+gridSize_ && center_.x() > coinItem_->x()-gridSize_)
    {
        if(center_.y() < coinItem_->y()+gridSize_ && center_.y() > coinItem_->y()-gridSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return 4;
        }
    }
    qDebug() << "you drag item: (" << item << ")";
    return 0;
}

void Redactor::setCursorStyle() {
    if (isdrawing_) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void Redactor::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isdrawing_ = true;
        QPoint localMousePos = event->pos();  // Получаем локальные координаты мыши
        center_ = view->mapToScene(localMousePos);
        center_.setX(center_.x() - gridSize_ / 2);
        center_.setY(center_.y() - gridSize_ / 2);
        setCursorStyle();
        qDebug() << "cursor Coordinates: (" << center_.x() << ", " << center_.y() << ")";
        dragElement_ = getElement();
        qDebug() << "you drag item: (" << getElement() << ")";
        update();
    }
}

void Redactor::mouseMoveEvent(QMouseEvent *event) {
    if (isdrawing_) {
        center_ = event->pos();
        update();
        qDebug() << "cursor Coordinates: (" << center_.x() << ", " << center_.y() << ")";
    }
}

void Redactor::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isdrawing_) {
        isdrawing_ = false;
        setCursorStyle();
        scene->removeItem(dragItem_);
        delete dragItem_;
        setupGameGrid();
    }
}
