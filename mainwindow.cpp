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
    : QMainWindow(parent), ui(new Ui::MainWindow) {


    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    int uiWidth = (cols_+4) * gridSize_;
    int uiHeight = (rows_+1) * gridSize_;

    this->setFixedSize(uiWidth, uiHeight);
    setCentralWidget(view);

    gameTimer_ = new QTimer(this);
    gameTimer_->start(250);
    connect(gameTimer_, SIGNAL(timeout()), this, SLOT(updateGameTime()));

    hostileRunTimer_ = new QTimer(this);
    hostileRunTimer_->start(1000);
    connect(hostileRunTimer_, SIGNAL(timeout()), this, SLOT(updateHostileRunTime()));


    gameGrid_ = new int*[rows_];
    for (int i = 0; i < rows_; ++i) {
        gameGrid_[i] = new int[cols_];
    }
        generateRandomGameGrid();

    setupGameGrid();
}

MainWindow::MainWindow(QWidget *parent, int** gameGrid)
    : QMainWindow(parent), ui(new Ui::MainWindow) {


    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    hostiles_.reserve(cols_*rows_);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    int uiWidth = (cols_+4) * gridSize_;
    int uiHeight = (rows_+1) * gridSize_;

    this->setFixedSize(uiWidth, uiHeight);
    setCentralWidget(view);

    gameTimer_ = new QTimer(this);
    gameTimer_->start(250);
    connect(gameTimer_, SIGNAL(timeout()), this, SLOT(updateGameTime()));

    hostileRunTimer_ = new QTimer(this);
    hostileRunTimer_->start(1000);
    connect(hostileRunTimer_, SIGNAL(timeout()), this, SLOT(updateHostileRunTime()));

    if (gameGrid != nullptr) {
        countCoins_ = 0;
        std::vector<std::vector<int>> matrix;
            for (int i = 0; i < rows_; ++i) {
                std::vector<int> row;
                for (int j = 0; j < cols_; ++j) {
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
    if (gameGrid != nullptr) {
            gameGrid_ = new int*[rows_];
            for (int i = 0; i < rows_; ++i) {
                gameGrid_[i] = new int[cols_];
                for (int j = 0; j < cols_; ++j) {
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
                        countCoins_++;
                    }
                    if(gameGrid_[i][j] == 2)
                    {
                        player_.setY(i);
                        player_.setX(j);
                    }
                }
            }
        } else {
        gameGrid_ = new int*[rows_];
        for (int i = 0; i < rows_; ++i) {
            gameGrid_[i] = new int[cols_];
        }
        generateRandomGameGrid();
    }

    setupGameGrid();
}

void MainWindow::updateGameTime() {
    gameTime_ += 0.5;
    isKeyTime_ = true;
    setupGameGrid();
    if(gameGrid_[player_.getY()][player_.getX()] == 3)
    {
        gameOver(false);
    }
    if(countCoins_ == 0)
    {
        gameOver(true);
    }
}

void MainWindow::updateHostileRunTime() {
    moveHostile();
}

MainWindow::~MainWindow(){
    for (int i = 0; i < rows_; ++i) {
        delete[] gameGrid_[i];
    }
    delete[] gameGrid_;

    delete ui;
}

void MainWindow::gameOver(bool isWin)
{
    disconnect(gameTimer_, SIGNAL(timeout()), this, SLOT(updateGameTime()));
    disconnect(hostileRunTimer_, SIGNAL(timeout()), this, SLOT(updateHostileRunTime()));
    if(isWin)
    {
        std::string str1 = "You WIN!";
        const char* charArray = str1.c_str();
        QMessageBox::information(this, "WIN", charArray);
    }
    else
    {
        std::string str1 = "Game Over, coins left: ";
        std::string str2 = std::to_string(countCoins_);
        str1.append(str2);
        const char* charArray = str1.c_str();
        QMessageBox::information(this, "False", charArray);
    }
    close();
    handleGameOver();
}

void MainWindow::handleGameOver() {
    StartGame *SG = new StartGame();
    SG->show();
}

void MainWindow::generateRandomGameGrid() {

    std::srand(std::time(0));

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            gameGrid_[i][j] = 0;
        }
    }

    int randomRow = std::rand() % rows_;
    int randomCol = std::rand() % cols_;

    gameGrid_[randomRow][randomCol] = 2;
    player_.setX(randomCol);
    player_.setY(randomRow);
    qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";

    int element = 1;
    int count = 5;
    generateRandomElements(element,count);

    element = 3;
    count = 2;
    generateRandomElements(element,count);

    element = 4;
    generateRandomElements(element,countCoins_);

}

void MainWindow::generateRandomElements(int element, int count) {
    Hostile newHostile;
    Point p(0,0);
    if(element == 3){
        std::vector<std::vector<int>> matrix;
            for (int i = 0; i < rows_; ++i) {
                std::vector<int> row;
                for (int j = 0; j < cols_; ++j) {
                    row.push_back(gameGrid_[i][j]);
                }
                matrix.push_back(row);
            }

            for (const auto& row : matrix) {
                for (const auto& element : row) {
                    std::cout << element << " ";
                }
                std::cout << std::endl;
            }
            newHostile.setMatrix(matrix);
    }
    for (int k = 0; k < count; ++k) {
        int randomRow = std::rand() % rows_;
        int randomCol = std::rand() % cols_;

        while (gameGrid_[randomRow][randomCol] != 0) {
        randomRow = std::rand() % rows_;
        randomCol = std::rand() % cols_;
        }
        if(element == 3)
        {
            p.x = randomCol;
            p.y = randomRow;
            newHostile.setPosition(p);
            hostiles_.push_back(newHostile);
        }
        gameGrid_[randomRow][randomCol] = element;
    }
}

void MainWindow::setupGameGrid() {
        scene->clear();
        QGraphicsPixmapItem* puckmanItem;
        QGraphicsPixmapItem* hostileItem;
        QGraphicsPixmapItem* coinItem;
        QGraphicsTextItem* coinsLabel = scene->addText("Coins: "+ QString::number(countCoins_));
        coinsLabel->setDefaultTextColor(Qt::black);
        QFont font = coinsLabel->font();
        font.setPointSize(14);
        coinsLabel->setFont(font);
        coinsLabel->setPos(cols_ * gridSize_ + 10, 10);
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
                case 4: // coin
                    coinItem = scene->addPixmap(QPixmap(":resource/coin.png").scaled(gridSize_, gridSize_));
                    coinItem->setX(j * gridSize_);
                    coinItem->setY(i * gridSize_);
                    break;
                }
            }
        }

        view->setSceneRect(0, 0, cols_ * gridSize_, rows_ * gridSize_);

        view->update();
}

void MainWindow::moveHostile()
{
    if (hostiles_.empty()) {
        qDebug() << "No hostiles found.";
        return;
    }
    std::vector<std::vector<int>> matrix;
        for (int i = 0; i < rows_; ++i) {
            std::vector<int> row;
            for (int j = 0; j < cols_; ++j) {
                row.push_back(gameGrid_[i][j]);
            }
            matrix.push_back(row);
        }

        for (const auto& row : matrix) {
            for (const auto& element : row) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }

    std::vector<char> direction;
    Point playerPoint(player_.getX(), player_.getY());
    Point hostilePosition(0,0);

    for (Hostile& currentHostile : hostiles_) {
        currentHostile.setMatrix(matrix);
        direction = currentHostile.getPath(playerPoint);
        if(direction.empty())
        {
            return;
        }
        if(direction[0] == 'L')
        {
            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.temp;
            currentHostile.temp = gameGrid_[hostilePosition.y][hostilePosition.x-1];
            hostilePosition.x = hostilePosition.x - 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            currentHostile.setPosition(hostilePosition);
        }
        if(direction[0] == 'R')
        {
            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.temp;
            currentHostile.temp = gameGrid_[hostilePosition.y][hostilePosition.x+1];
            hostilePosition.x = hostilePosition.x + 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            currentHostile.setPosition(hostilePosition);
        }
        if(direction[0] == 'U')
        {

            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.temp;
            currentHostile.temp = gameGrid_[hostilePosition.y-1][hostilePosition.x];
            hostilePosition.y = hostilePosition.y - 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            qDebug() << "gameGrid_ Coordinates: (" << hostilePosition.x << ", " << hostilePosition.y << ")";
            currentHostile.setPosition(hostilePosition);
        }
        if(direction[0] == 'D')
        {

            hostilePosition = currentHostile.getPosition();
            gameGrid_[hostilePosition.y][hostilePosition.x] = currentHostile.temp;
            currentHostile.temp = gameGrid_[hostilePosition.y+1][hostilePosition.x];
            hostilePosition.y = hostilePosition.y + 1;
            gameGrid_[hostilePosition.y][hostilePosition.x] = 3;
            qDebug() << "gameGrid_ Coordinates: (" << hostilePosition.x << ", " << hostilePosition.y << ")";
            currentHostile.setPosition(hostilePosition);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(isKeyTime_)
    {
        switch (event->key()) {
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

void MainWindow::movePlayerUp() {
    if (player_.getY() > 0) {
        if(gameGrid_[player_.getY()-1][player_.getX()] != 1)
        {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            if(gameGrid_[player_.getY()-1][player_.getX()] == 4)
            {
                countCoins_--;
            }
            gameGrid_[player_.getY()-1][player_.getX()] = 2;
            player_.setY(player_.getY() - 1);
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerDown() {
    if (player_.getY()+1 < rows_) {
        if(gameGrid_[player_.getY()+1][player_.getX()] != 1)
        {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            if(gameGrid_[player_.getY()+1][player_.getX()] == 4)
            {
                countCoins_--;
            }
            gameGrid_[player_.getY()+1][player_.getX()] = 2;
            player_.setY(player_.getY() + 1);
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerLeft() {
    if(gameGrid_[player_.getY()][player_.getX()-1] != 1)
    {
        if (player_.getX() > 0) {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            if(gameGrid_[player_.getY()][player_.getX()-1] == 4)
            {
                countCoins_--;
            }
            gameGrid_[player_.getY()][player_.getX()-1] = 2;
            player_.setX(player_.getX() - 1);
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerRight() {
    if (player_.getX()+1 < cols_) {
        if(gameGrid_[player_.getY()][player_.getX()+1] != 1)
        {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            if(gameGrid_[player_.getY()][player_.getX()+1] == 4)
            {
                countCoins_--;
            }
            gameGrid_[player_.getY()][player_.getX()+1] = 2;
            player_.setX(player_.getX() + 1);
            qDebug() << "Player Coordinates: (" << player_.getY() << ", " << player_.getX() << ")";
        }
    }
}
