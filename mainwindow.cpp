#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hostile.h"
#include "startgame.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_, this);

    int uiWidth = (kColumns_ + 4) * cellSize_;
    int uiHeight = (kRows_ + 1) * cellSize_;

    this->setFixedSize(uiWidth, uiHeight);
    setCentralWidget(view_);

    gameTimer_ = new QTimer(this);
    gameTimer_->start(250);
    connect(gameTimer_, SIGNAL(timeout()), this, SLOT(updateGameTime()));

    hostileRunTimer_ = new QTimer(this);
    hostileRunTimer_->start(1000);
    connect(hostileRunTimer_, SIGNAL(timeout()), this, SLOT(updateHostileRunTime()));

    gameGrid_ = new int*[kRows_];
    for (int i = 0; i < kRows_; ++i)
    {
        gameGrid_[i] = new int[kColumns_];
    }
        generateRandomGameGrid();

    setupGameGrid();
}

MainWindow::MainWindow(QWidget *parent, int** gameGrid)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    hostiles_.reserve(kColumns_*kRows_);

    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_, this);

    int uiWidth = (kColumns_ + 4) * cellSize_;
    int uiHeight = (kRows_ + 1) * cellSize_;

    this->setFixedSize(uiWidth, uiHeight);
    setCentralWidget(view_);

    gameTimer_ = new QTimer(this);
    gameTimer_->start(250);
    connect(gameTimer_, SIGNAL(timeout()), this, SLOT(updateGameTime()));

    hostileRunTimer_ = new QTimer(this);
    hostileRunTimer_->start(1000);
    connect(hostileRunTimer_, SIGNAL(timeout()), this, SLOT(updateHostileRunTime()));

    if (gameGrid != nullptr)
    {
        counsCount_ = 0;
        std::vector<std::vector<int>> matrix;
            for (int i = 0; i < kRows_; ++i)
            {
                std::vector<int> row;
                for (int j = 0; j < kColumns_; ++j)
                {
                    row.push_back(gameGrid[i][j]);
                }
                matrix.push_back(row);
            }
            for (const auto& row : matrix) {
                for (const auto& element : row) {
                    std::cout << element << " ";
                }
                std::cout << std::endl;
            }
    }
    Hostile newHostile;
    Point p(0, 0);
    if (gameGrid != nullptr)
    {
            gameGrid_ = new int*[kRows_];
            for (int i = 0; i < kRows_; ++i)
            {
                gameGrid_[i] = new int[kColumns_];
                for (int j = 0; j < kColumns_; ++j)
                {
                    gameGrid_[i][j] = gameGrid[i][j];
                    if(gameGrid_[i][j] == 3)
                    {
                        p.x = j;
                        p.y = i;
                        newHostile.setPosition(p);
                        qDebug() << "Hostile Coordinates: (" << p.x << ", " << p.y << ")";
                        hostiles_.push_back(newHostile);
                    }
                    if(gameGrid_[i][j] == 4)
                    {
                        counsCount_++;
                    }
                    if(gameGrid_[i][j] == 2)
                    {
                        player_.setY(i);
                        player_.setX(j);
                    }
                }
            }
        }
    else
    {
        gameGrid_ = new int*[kRows_];
        for (int i = 0; i < kRows_; ++i)
        {
            gameGrid_[i] = new int[kColumns_];
        }
        generateRandomGameGrid();
    }

    setupGameGrid();
}

void MainWindow::updateGameTime()
{
    gameTime_ += 0.5;
    isKeyTime_ = true;
    setupGameGrid();
    if(gameGrid_[player_.getY()][player_.getX()] == 3)
    {
        gameOver(false);
    }
    if(counsCount_ == 0)
    {
        gameOver(true);
    }
}

void MainWindow::updateHostileRunTime()
{
    moveHostile();
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < kRows_; ++i)
    {
        delete[] gameGrid_[i];
    }
    delete[] gameGrid_;

    delete ui;
}

void MainWindow::gameOver(bool isWin)
{
    disconnect(gameTimer_, SIGNAL(timeout()), this, SLOT(updateGameTime()));
    disconnect(hostileRunTimer_, SIGNAL(timeout()), this, SLOT(updateHostileRunTime()));
    if (isWin)
    {
        std::string str1 = "You WIN!";
        const char* charArray = str1.c_str();
        QMessageBox::information(this, "WIN", charArray);
    }
    else
    {
        std::string str1 = "Game Over, coins left: ";
        std::string str2 = std::to_string(counsCount_);
        str1.append(str2);
        const char* charArray = str1.c_str();
        QMessageBox::information(this, "False", charArray);
    }
    close();
    handleGameOver();
}

void MainWindow::handleGameOver()
{
    StartGame *SG = new StartGame();
    SG->show();
}

void MainWindow::generateRandomGameGrid()
{

    std::srand(std::time(0));

    for (int i = 0; i < kRows_; ++i)
    {
        for (int j = 0; j < kColumns_; ++j)
        {
            gameGrid_[i][j] = 0;
        }
    }

    int randomRow = std::rand() % kRows_;
    int randomColumn = std::rand() % kColumns_;

    gameGrid_[randomRow][randomColumn] = 2;
    player_.setX(randomColumn);
    player_.setY(randomRow);
    qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";

    int element = 1;
    int count = 5;
    generateRandomElements(element,count);

    element = 3;
    count = 2;
    generateRandomElements(element,count);

    element = 4;
    generateRandomElements(element,counsCount_);

}

void MainWindow::generateRandomElements(int element, int count)
{
    Hostile newHostile;
    Point p(0,0);
    if (element == 3)
    {
        std::vector<std::vector<int>> matrix;
            for (int i = 0; i < kRows_; ++i)
            {
                std::vector<int> row;
                for (int j = 0; j < kColumns_; ++j)
                {
                    row.push_back(gameGrid_[i][j]);
                }
                matrix.push_back(row);
            }

            for (const auto& row : matrix)
            {
                for (const auto& element : row)
                {
                    std::cout << element << " ";
                }
                std::cout << std::endl;
            }
            newHostile.setMatrix(matrix);
    }
    for (int k = 0; k < count; ++k)
    {
        int randomRow = std::rand() % kRows_;
        int randomColumn = std::rand() % kColumns_;

        while (gameGrid_[randomRow][randomColumn] != 0)
        {
            randomRow = std::rand() % kRows_;
            randomColumn = std::rand() % kColumns_;
        }
        if(element == 3)
        {
            p.x = randomColumn;
            p.y = randomRow;
            newHostile.setPosition(p);
            hostiles_.push_back(newHostile);
        }
        gameGrid_[randomRow][randomColumn] = element;
    }
}

void MainWindow::setupGameGrid()
{
        scene_->clear();
        QGraphicsPixmapItem* puckmanItem;
        QGraphicsPixmapItem* hostileItem;
        QGraphicsPixmapItem* coinItem;
        QGraphicsTextItem* coinsLabel = scene_->addText("Coins: "+ QString::number(counsCount_));
        coinsLabel->setDefaultTextColor(Qt::black);
        QFont font = coinsLabel->font();
        font.setPointSize(14);
        coinsLabel->setFont(font);
        coinsLabel->setPos(kColumns_ * cellSize_ + 10, 10);
        for (int i = 0; i < kRows_; ++i)
        {
            for (int j = 0; j < kColumns_; ++j)
            {

                switch (gameGrid_[i][j])
                {
                case GameElement::Empty:
                    scene_->addRect(j * cellSize_, i * cellSize_, cellSize_, cellSize_, QPen(Qt::black), QBrush(Qt::white));
                    break;
                case GameElement::Wall:
                    scene_->addRect(j * cellSize_, i * cellSize_, cellSize_, cellSize_, QPen(Qt::black), QBrush(Qt::darkGray));
                    break;
                case GameElement::Puckman:
                    puckmanItem = scene_->addPixmap(QPixmap(":resource/puckman.png").scaled(cellSize_, cellSize_));
                    puckmanItem->setX(j * cellSize_);
                    puckmanItem->setY(i * cellSize_);
                    break;
                case GameElement::Hostile:
                    hostileItem = scene_->addPixmap(QPixmap(":resource/hostile.png").scaled(cellSize_, cellSize_));
                    hostileItem->setX(j * cellSize_);
                    hostileItem->setY(i * cellSize_);
                    break;
                case GameElement::Coin:
                    coinItem = scene_->addPixmap(QPixmap(":resource/coin.png").scaled(cellSize_, cellSize_));
                    coinItem->setX(j * cellSize_);
                    coinItem->setY(i * cellSize_);
                    break;
                }
            }
        }

        view_->setSceneRect(0, 0, kColumns_ * cellSize_, kRows_ * cellSize_);

        view_->update();
}

void MainWindow::moveHostile()
{
    if (hostiles_.empty())
    {
        qDebug() << "No hostiles found.";
        return;
    }
    std::vector<std::vector<int>> matrix;
        for (int i = 0; i < kRows_; ++i)
        {
            std::vector<int> row;
            for (int j = 0; j < kColumns_; ++j)
            {
                row.push_back(gameGrid_[i][j]);
            }
            matrix.push_back(row);
        }

        for (const auto& row : matrix)
        {
            for (const auto& element : row)
            {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }

    std::vector<Directions> direction;
    Point playerPoint(player_.getX(), player_.getY());
    Point hostilePosition(0,0);

    for (Hostile& currentHostile : hostiles_)
    {
        currentHostile.setMatrix(matrix);
        direction = currentHostile.getPath(playerPoint);
        if (direction.empty())
        {
            return;
        }
        if (direction[0] == Directions::Left)
        {
            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.getPreviousElement();
            currentHostile.setPreviousElement(gameGrid_[hostilePosition.y][hostilePosition.x - 1]);
            hostilePosition.x = hostilePosition.x - 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            currentHostile.setPosition(hostilePosition);
        }
        if (direction[0] == Directions::Right)
        {
            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.getPreviousElement();
            currentHostile.setPreviousElement(gameGrid_[hostilePosition.y][hostilePosition.x + 1]);
            hostilePosition.x = hostilePosition.x + 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            currentHostile.setPosition(hostilePosition);
        }
        if (direction[0] == Directions::Up)
        {

            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.getPreviousElement();
            currentHostile.setPreviousElement(gameGrid_[hostilePosition.y-1][hostilePosition.x]);
            hostilePosition.y = hostilePosition.y - 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            qDebug() << "gameGrid_ Coordinates: (" << hostilePosition.x << ", " << hostilePosition.y << ")";
            currentHostile.setPosition(hostilePosition);
        }
        if (direction[0] == Directions::Down)
        {

            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.getPreviousElement();
            currentHostile.setPreviousElement(gameGrid_[hostilePosition.y+1][hostilePosition.x]);
            hostilePosition.y = hostilePosition.y + 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            qDebug() << "gameGrid_ Coordinates: (" << hostilePosition.x << ", " << hostilePosition.y << ")";
            currentHostile.setPosition(hostilePosition);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (isKeyTime_)
    {
        switch (event->key())
        {
            case Qt::Key_W:
                movePlayerUp();
                isKeyTime_ = false;
                break;
            case Qt::Key_S:
                movePlayerDown();
                isKeyTime_ = false;
                break;
            case Qt::Key_A:
                movePlayerLeft();
                isKeyTime_ = false;
                break;
            case Qt::Key_D:
                movePlayerRight();
                isKeyTime_ = false;
                break;
            default:
                QMainWindow::keyPressEvent(event);
                break;
        }
    }
}

void MainWindow::movePlayerUp()
{
    if (player_.getY() > 0)
    {
        if (gameGrid_[player_.getY() - 1][player_.getX()] != GameElement::Wall)
        {
            gameGrid_[player_.getY()][player_.getX()] = GameElement::Empty;
            if (gameGrid_[player_.getY() - 1][player_.getX()] == GameElement::Coin)
            {
                counsCount_--;
            }
            gameGrid_[player_.getY() - 1][player_.getX()] = GameElement::Puckman;
            player_.setY(player_.getY() - 1);
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerDown()
{
    if (player_.getY()+1 < kRows_)
    {
        if (gameGrid_[player_.getY()+1][player_.getX()] != GameElement::Wall)
        {
            gameGrid_[player_.getY()][player_.getX()] = GameElement::Empty;
            if (gameGrid_[player_.getY()+1][player_.getX()] == GameElement::Coin)
            {
                counsCount_--;
            }
            gameGrid_[player_.getY()+1][player_.getX()] = GameElement::Puckman;
            player_.setY(player_.getY() + 1);
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerLeft()
{
    if (gameGrid_[player_.getY()][player_.getX() - 1] != GameElement::Wall)
    {
        if (player_.getX() > 0)
        {
            gameGrid_[player_.getY()][player_.getX()] = GameElement::Empty;
            if (gameGrid_[player_.getY()][player_.getX() - 1] == GameElement::Coin)
            {
                counsCount_--;
            }
            gameGrid_[player_.getY()][player_.getX() - 1] = GameElement::Puckman;
            player_.setX(player_.getX() - 1);
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerRight()
{
    if (player_.getX() + 1 < kColumns_)
    {
        if (gameGrid_[player_.getY()][player_.getX() + 1] != GameElement::Wall)
        {
            gameGrid_[player_.getY()][player_.getX()] = GameElement::Empty;
            if (gameGrid_[player_.getY()][player_.getX() + 1] == GameElement::Coin)
            {
                counsCount_--;
            }
            gameGrid_[player_.getY()][player_.getX() + 1] = GameElement::Puckman;
            player_.setX(player_.getX() + 1);
            qDebug() << "Player Coordinates: (" << player_.getY() << ", " << player_.getX() << ")";
        }
    }
}
