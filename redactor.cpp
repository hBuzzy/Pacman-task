#include "redactor.h"
#include "ui_redactor.h"
#include "mainwindow.h"

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
    view = new CustomGraphicsView(scene, this, gridSize_);

    view->setMouseTracking(true);
    view->hasMouseTracking();

    connect(view, &CustomGraphicsView::customMouseRelease, this, &Redactor::handleCustomMouseRelease);

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

    myButton = new QPushButton("Начать игру", this);
    myButton->setGeometry(10, 10, 100, 30); // Установите положение и размер кнопки
    connect(myButton, &QPushButton::clicked, this, &Redactor::handleButtonClick);

    setupGameGrid();

    ui->setupUi(this);
}

void Redactor::exitRedaction()
{
    close();
    openGame();
}

void Redactor::openGame() {
    MainWindow *SG = new MainWindow(nullptr, gameGrid_);
    SG->show();
}

void Redactor::handleButtonClick() {
    exitRedaction();
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
    dragItem();
}

void Redactor::dragItem() {
    if (dragItem_ != nullptr) {
        scene->removeItem(dragItem_);
        delete dragItem_;
    }
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
    dragItem_->setX(view->center.x());
    dragItem_->setY(view->center.y());
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
}

QPoint Redactor::getGridPoint() {
    QPoint a(-1, -1);
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            if(view->center.x() > i*gridSize_-gridSize_/2 && view->center.x() < i*gridSize_+gridSize_/2)
            {
                if(view->center.y() > j*gridSize_-gridSize_/2 && view->center.y() < j*gridSize_+gridSize_/2)
                {
                    a.setX(i);
                    a.setY(j);
                    qDebug() << "getGridPoint Coordinates: (" << a.x() << ", " << a.y() << ")";
                    return(a);
                }
            }
        }
    }
    qDebug() << "getGridPoint Coordinates: (" << a.x() << ", " << a.y() << ")";
    return a;

}

int Redactor::getElement() {
    QGraphicsItem* item = scene->itemAt(view->center.x(), view->center.y(), QTransform());
    if(view->center.x() < wallItem_->x()+gridSize_ && view->center.x() > wallItem_->x()-gridSize_)
    {
        if(view->center.y() < wallItem_->y()+gridSize_ && view->center.y() > wallItem_->y()-gridSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return 1;
        }
    }
    if(view->center.x() < puckmanItem_->x()+gridSize_ && view->center.x() > puckmanItem_->x()-gridSize_)
    {
        if(view->center.y() < puckmanItem_->y()+gridSize_ && view->center.y() > puckmanItem_->y()-gridSize_)
        {
            if(isStatePacmanElement_ == false)
            {
                qDebug() << "you drag item: (" << item << ")";
                return 2;
            }
        }
    }
    if(view->center.x() < hostileItem_->x()+gridSize_ && view->center.x() > hostileItem_->x()-gridSize_)
    {
        if(view->center.y() < hostileItem_->y()+gridSize_ && view->center.y() > hostileItem_->y()-gridSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return 3;
        }
    }
    if(view->center.x() < coinItem_->x()+gridSize_ && view->center.x() > coinItem_->x()-gridSize_)
    {
        if(view->center.y() < coinItem_->y()+gridSize_ && view->center.y() > coinItem_->y()-gridSize_)
        {
            qDebug() << "you drag item: (" << item << ")";
            return 4;
        }
    }
    qDebug() << "you drag item: (" << item << ")";
    return 0;
}

void Redactor::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragItem_ = new QGraphicsPixmapItem();
        view->isdrawing = true;
        QPoint localMousePos = event->pos();  // Получаем локальные координаты мыши
        view->center = view->mapToScene(localMousePos);
        view->center.setX(view->center.x());
        view->center.setY(view->center.y());
        view->setCursorStyle();
        qDebug() << "cursor Coordinates: (" << view->center.x() << ", " << view->center.y() << ")";
        dragElement_ = getElement();
        qDebug() << "you drag item: (" << getElement() << ")";

        update();
    }

}

void Redactor::handleCustomMouseRelease() {
    dragItem_ = new QGraphicsPixmapItem();
    QPoint a = getGridPoint();
    if(a.x() > 0 || a.y() > 0)
    {
        if (gameGrid_[a.y()][a.x()] == 2)
        {
            isStatePacmanElement_ = false;
        }
        gameGrid_[a.y()][a.x()] = dragElement_;
        if(dragElement_ == 2)
        {
            isStatePacmanElement_ = true;
        }
    }
    update();
    setupGameGrid();
}
