#include "redactor.h"
#include "ui_redactor.h"
#include "mainwindow.h"
#include "startgame.h"

#include <iostream>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

Redactor::Redactor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Redactor)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    scene = new QGraphicsScene(this);
    view = new CustomGraphicsView(scene, this, cellSize_);

    view->setMouseTracking(true);
    view->hasMouseTracking();

    connect(view, &CustomGraphicsView::customMouseRelease, this, &Redactor::handleCustomMouseRelease);

    int indentationColumns = 6;
    int indentationRows = 1;
    int uiWidth = (kColumns_ + indentationColumns) * cellSize_;
    int uiHeight = (kRows_ + indentationRows) * cellSize_;

    this->setFixedSize(uiWidth, uiHeight);
    setCentralWidget(view);

    gameGrid_ = new int*[kRows_];
    for (int i = 0; i < kRows_; ++i)
    {
        gameGrid_[i] = new int[kColumns_];
    }
    for (int i = 0; i < kRows_; ++i)
    {
        for (int j = 0; j < kColumns_; ++j)
        {
            gameGrid_[i][j] = 0;
        }
    }

    myButton = new QPushButton("Начать игру", this);
    myButton->setGeometry(10, 10, 100, 30);
    connect(myButton, &QPushButton::clicked, this, &Redactor::handleButtonClick);

    setupGameGrid();

    ui->setupUi(this);
}

void Redactor::exitRedaction()
{
    close();
    openGame();
}

void Redactor::openGame()
{
    MainWindow *startGame = new MainWindow(nullptr, gameGrid_);
    startGame->show();
}

void Redactor::handleButtonClick()
{
    if (counsCount_ <= 0)
    {
        std::string stringWarn = "couns count < 0";
        const char* charArray = stringWarn.c_str();
        QMessageBox::information(this, "False", charArray);
        return;
    }
    exitRedaction();
}

Redactor::~Redactor()
{
    for (int i = 0; i < kRows_; ++i)
    {
        delete[] gameGrid_[i];
    }
    delete[] gameGrid_;
    delete ui;
}

void Redactor::paintEvent(QPaintEvent *event)
{
    dragItem();
}

void Redactor::dragItem()
{
    if (dragItem_ != nullptr)
    {
        scene->removeItem(dragItem_);
        delete dragItem_;
    }
    dragItem_ = new QGraphicsPixmapItem();
    if (dragElement_ == GameElement::Wall)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/wall.png").scaled(cellSize_, cellSize_));
    }
    if (dragElement_ == GameElement::Puckman)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/puckman.png").scaled(cellSize_, cellSize_));
    }
    if (dragElement_ == GameElement::Hostile)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/hostile.png").scaled(cellSize_, cellSize_));
    }
    if (dragElement_ == GameElement::Coin)
    {
        dragItem_ = scene->addPixmap(QPixmap(":resource/coin.png").scaled(cellSize_, cellSize_));
    }
    dragItem_->setX(view->getCenterX());
    dragItem_->setY(view->getCenterY());
}

void Redactor::setupGameGrid()
{
        scene->clear();
        QGraphicsPixmapItem* puckmanItem;
        QGraphicsPixmapItem* hostileItem;
        QGraphicsPixmapItem* coinItem;

        for (int i = 0; i < kRows_; ++i)
        {
            for (int j = 0; j < kColumns_; ++j)
            {

                switch (gameGrid_[i][j])
                {
                case GameElement::Empty:
                    scene->addRect(j * cellSize_, i * cellSize_, cellSize_, cellSize_, QPen(Qt::black), QBrush(Qt::white));
                    break;
                case GameElement::Wall:
                    scene->addRect(j * cellSize_, i * cellSize_, cellSize_, cellSize_, QPen(Qt::black), QBrush(Qt::darkGray));
                    break;
                case GameElement::Puckman:
                    puckmanItem = scene->addPixmap(QPixmap(":resource/puckman.png").scaled(cellSize_, cellSize_));
                    puckmanItem->setX(j * cellSize_);
                    puckmanItem->setY(i * cellSize_);
                    break;
                case GameElement::Hostile:
                    hostileItem = scene->addPixmap(QPixmap(":resource/hostile.png").scaled(cellSize_, cellSize_));
                    hostileItem->setX(j * cellSize_);
                    hostileItem->setY(i * cellSize_);
                    break;
                case GameElement::Coin:
                    coinItem = scene->addPixmap(QPixmap(":resource/coin.png").scaled(cellSize_, cellSize_));
                    coinItem->setX(j * cellSize_);
                    coinItem->setY(i * cellSize_);
                    break;
                }
            }
        }
        int extraColumns = kColumns_+2;
        int Row = 2;
        int nextRow = Row * cellSize_;
        wallItem_ = scene->addPixmap(QPixmap(":resource/wall.png").scaled(cellSize_, cellSize_));
        wallItem_->setX((extraColumns) * cellSize_);
        wallItem_->setY(nextRow);
        qDebug() << "wallItem Coordinates: (" << wallItem_->x() << ", " << wallItem_->y() << ")";
        Row = 4;
        nextRow = Row * cellSize_;

        puckmanItem_ = scene->addPixmap(QPixmap(":resource/puckman.png").scaled(cellSize_, cellSize_));
        puckmanItem_->setX((extraColumns) * cellSize_);
        puckmanItem_->setY(nextRow);
        qDebug() << "puckmanItem Coordinates: (" << puckmanItem_->x() << ", " << puckmanItem_->y() << ")";

        Row = 6;
        nextRow = Row * cellSize_;

        hostileItem_ = scene->addPixmap(QPixmap(":resource/hostile.png").scaled(cellSize_, cellSize_));
        hostileItem_->setX((extraColumns) * cellSize_);
        hostileItem_->setY(nextRow);
        qDebug() << "hostileItem Coordinates: (" << hostileItem_->x() << ", " << hostileItem_->y() << ")";

        Row = 8;
        nextRow = Row * cellSize_;

        coinItem_ = scene->addPixmap(QPixmap(":resource/coin.png").scaled(cellSize_, cellSize_));
        coinItem_->setX((extraColumns) * cellSize_);
        coinItem_->setY(nextRow);
        qDebug() << "coinItem Coordinates: (" << coinItem_->x() << ", " << coinItem_->y() << ")";

        view->setSceneRect(0, 0, kColumns_ * cellSize_, kRows_ * cellSize_);

        view->update();
}

QPoint Redactor::getGridPoint()
{
    QPoint coordinateElementGrid(-1, -1);
    int halfGridSize = cellSize_ / 2;
    for (int i = 0; i < kRows_; ++i)
    {
        for (int j = 0; j < kColumns_; ++j)
        {
            if (view->getCenterX() > i * cellSize_ - halfGridSize && view->getCenterX()< i * cellSize_ + halfGridSize)
            {
                if (view->getCenterY() > j * cellSize_ - halfGridSize && view->getCenterY() < j * cellSize_ + halfGridSize)
                {
                    coordinateElementGrid.setX(i);
                    coordinateElementGrid.setY(j);
                    qDebug() << "getGridPoint Coordinates: (" << coordinateElementGrid.x() << ", " << coordinateElementGrid.y() << ")";
                    return(coordinateElementGrid);
                }
            }
        }
    }
    qDebug() << "getGridPoint Coordinates: (" << coordinateElementGrid.x() << ", " << coordinateElementGrid.y() << ")";
    return coordinateElementGrid;
}

int Redactor::getElement()
{
    QGraphicsItem* item = scene->itemAt(view->getCenterX(), view->getCenterY(), QTransform());
    if (view->getCenterX() < wallItem_->x() + cellSize_ && view->getCenterX() > wallItem_->x() - cellSize_)
    {
        if (view->getCenterY() < wallItem_->y() + cellSize_ && view->getCenterY() > wallItem_->y() - cellSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return GameElement::Wall;
        }
    }
    if (view->getCenterX() < puckmanItem_->x() + cellSize_ && view->getCenterX() > puckmanItem_->x() - cellSize_)
    {
        if (view->getCenterY() < puckmanItem_->y() + cellSize_ && view->getCenterY() > puckmanItem_->y() - cellSize_)
        {
            if (isStatePacmanElement_ == false)
            {
                qDebug() << "you drag item: (" << item << ")";
                return GameElement::Puckman;
            }
        }
    }
    if (view->getCenterX() < hostileItem_->x() + cellSize_ && view->getCenterX() > hostileItem_->x() - cellSize_)
    {
        if (view->getCenterY() < hostileItem_->y() + cellSize_ && view->getCenterY() > hostileItem_->y() - cellSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return GameElement::Hostile;
        }
    }
    if (view->getCenterX() < coinItem_->x() + cellSize_ && view->getCenterX() > coinItem_->x() - cellSize_)
    {
        if (view->getCenterY() < coinItem_->y() + cellSize_ && view->getCenterY() > coinItem_->y() - cellSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return GameElement::Coin;
        }
    }
    qDebug() << "you drag item: (" << item << ")";
    return 0;
}

void Redactor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragItem_ = new QGraphicsPixmapItem();
        view->setDrawing(true);
        QPoint localMousePos = event->pos();
        view->setCenter(view->mapToScene(localMousePos));
        view->setCenterX(view->getCenterX());
        view->setCenterY(view->getCenterY());
        view->setCursorStyle();
        qDebug() << "cursor Coordinates: (" << view->getCenterX()<< ", " << view->getCenterY() << ")";
        dragElement_ = getElement();
        qDebug() << "you drag item: (" << getElement() << ")";

        update();
    }

}

void Redactor::handleCustomMouseRelease()
{
    dragItem_ = new QGraphicsPixmapItem();
    QPoint cell = getGridPoint();
    if (cell.x() > 0 || cell.y() > 0)
    {
        if (gameGrid_[cell.y()][cell.x()] == GameElement::Puckman)
        {
            isStatePacmanElement_ = false;
        }
        if (gameGrid_[cell.y()][cell.x()] == GameElement::Coin)
        {
            counsCount_--;
        }
        gameGrid_[cell.y()][cell.x()] = dragElement_;
        if (dragElement_ == GameElement::Puckman)
        {
            isStatePacmanElement_ = true;
        }
        if (dragElement_ == GameElement::Coin)
        {
            counsCount_++;
            qDebug() << "counsCount = (" << counsCount_ << ")";
        }
    }
    update();
    setupGameGrid();
}
