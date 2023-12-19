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

MainWindow::MainWindow(QWidget *parent, int** gameGrid)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    hostiles_.reserve(kColumns_*kRows_);

    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_, this);

    int indentationColumns = 4;
    int indentationRows = 4;
    int uiWidth = (kColumns_ + indentationColumns) * cellSize_;
    int uiHeight = (kRows_ + indentationRows) * cellSize_;

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
    Point hostilePosition(0, 0);
    if (gameGrid != nullptr)
    {
            gameGrid_ = new int*[kRows_];
            for (int i = 0; i < kRows_; ++i)
            {
                gameGrid_[i] = new int[kColumns_];
                for (int j = 0; j < kColumns_; ++j)
                {
                    gameGrid_[i][j] = gameGrid[i][j];
                    if(gameGrid_[i][j] == GameElement::Hostile)
                    {
                        hostilePosition.x = j;
                        hostilePosition.y = i;
                        newHostile.setPosition(hostilePosition);
                        qDebug() << "Hostile Coordinates: (" << hostilePosition.x << ", " << hostilePosition.y << ")";
                        hostiles_.push_back(newHostile);
                    }
                    if(gameGrid_[i][j] == GameElement::Coin)
                    {
                        counsCount_++;
                    }
                    if(gameGrid_[i][j] == GameElement::Puckman)
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
    if(gameGrid_[player_.getY()][player_.getX()] == GameElement::Hostile)
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
        std::string stringGameOver = "You WIN!";
        const char* charArray = stringGameOver.c_str();
        QMessageBox::information(this, "WIN", charArray);
    }
    else
    {
        std::string stringGameOver = "Game Over, coins left: ";
        std::string stringGameOver2 = std::to_string(counsCount_);
        stringGameOver.append(stringGameOver2);
        const char* charArray = stringGameOver.c_str();
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

    gameGrid_[randomRow][randomColumn] = GameElement::Puckman;
    player_.setX(randomColumn);
    player_.setY(randomRow);
    qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";

    int element = GameElement::Wall;
    int count = 5;
    generateRandomElements(element,count);

    element = GameElement::Hostile;
    count = 2;
    generateRandomElements(element,count);

    element = GameElement::Coin;
    generateRandomElements(element,counsCount_);

}

void MainWindow::generateRandomElements(int element, int count)
{
    Hostile newHostile;
    Point hostilePosition(0,0);
    if (element == GameElement::Hostile)
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

        while (gameGrid_[randomRow][randomColumn] != GameElement::Empty)
        {
            randomRow = std::rand() % kRows_;
            randomColumn = std::rand() % kColumns_;
        }
        if(element == GameElement::Hostile)
        {
            hostilePosition.x = randomColumn;
            hostilePosition.y = randomRow;
            newHostile.setPosition(hostilePosition);
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
            gameGrid_[hostilePosition.y][hostilePosition.x] = GameElement::Hostile;
            currentHostile.setPosition(hostilePosition);
        }
        if (direction[0] == Directions::Right)
        {
            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.getPreviousElement();
            currentHostile.setPreviousElement(gameGrid_[hostilePosition.y][hostilePosition.x + 1]);
            hostilePosition.x = hostilePosition.x + 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = GameElement::Hostile;
            currentHostile.setPosition(hostilePosition);
        }
        if (direction[0] == Directions::Up)
        {

            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.getPreviousElement();
            currentHostile.setPreviousElement(gameGrid_[hostilePosition.y - 1][hostilePosition.x]);
            hostilePosition.y = hostilePosition.y - 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = GameElement::Hostile;
            qDebug() << "gameGrid_ Coordinates: (" << hostilePosition.x << ", " << hostilePosition.y << ")";
            currentHostile.setPosition(hostilePosition);
        }
        if (direction[0] == Directions::Down)
        {

            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.getPreviousElement();
            currentHostile.setPreviousElement(gameGrid_[hostilePosition.y + 1][hostilePosition.x]);
            hostilePosition.y = hostilePosition.y + 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = GameElement::Hostile;
            qDebug() << "gameGrid_ Coordinates: (" << hostilePosition.x << ", " << hostilePosition.y << ")";
            currentHostile.setPosition(hostilePosition);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (isKeyTime_)
    {
        switch (event->key())
        {
            case Qt::Key_W:
                movePlayer(Directions::Up);
                isKeyTime_ = false;
                break;
            case Qt::Key_S:
                 movePlayer(Directions::Down);
                isKeyTime_ = false;
                break;
            case Qt::Key_A:
                 movePlayer(Directions::Left);
                isKeyTime_ = false;
                break;
            case Qt::Key_D:
                 movePlayer(Directions::Right);
                isKeyTime_ = false;
                break;
            default:
                QMainWindow::keyPressEvent(event);
                break;
        }
    }
}

void MainWindow::movePlayer(Directions direction)
{
    int newX = player_.getX();
    int newY = player_.getY();

    int direc = 4;
    if (direction == Directions::Up)
    {
        direc = 1;
    }
    if (direction == Directions::Down)
    {
        direc = 2;
    }
    if (direction == Directions::Left)
    {
        direc = 3;
    }
    switch (direc)
    {
    case 1:
        newY = std::max(0, newY - 1);
        break;
    case 2:
        newY = std::min(kRows_ - 1, newY + 1);
        break;
    case 3:
        newX = std::max(0, newX - 1);
        break;
    case 4:
        newX = std::min(kColumns_ - 1, newX + 1);
        break;
    }

    if (gameGrid_[newY][newX] != GameElement::Wall)
    {
        gameGrid_[player_.getY()][player_.getX()] = GameElement::Empty;
        if (gameGrid_[newY][newX] == GameElement::Coin)
        {
            counsCount_--;
        }
        gameGrid_[newY][newX] = GameElement::Puckman;
        player_.setX(newX);
        player_.setY(newY);
        qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
    }
}
