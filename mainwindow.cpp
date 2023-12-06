#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    tableWidget_ = ui->tableWidget;

    tableWidget_->setRowCount(rows_);
    tableWidget_->setColumnCount(cols_);

    tableWidget_->verticalHeader()->setDefaultSectionSize(32);
    tableWidget_->horizontalHeader()->setDefaultSectionSize(32);

    tableWidget_->verticalHeader()->setVisible(false);
    tableWidget_->horizontalHeader()->setVisible(false);

    tableWidget_->setVisible(true);

    // Выделите память для gameGrid
    gameGrid_ = new int*[rows_];
    for (int i = 0; i < rows_; ++i) {
        gameGrid_[i] = new int[cols_];
    }
    generateRandomGameGrid();
    setupGameGrid();
}

MainWindow::~MainWindow(){
    for (int i = 0; i < rows_; ++i) {
        delete[] gameGrid_[i];
    }
    delete[] gameGrid_;

    delete ui;
}

void MainWindow::generateRandomGameGrid() {
    // Сбросим генератор случайных чисел
    std::srand(std::time(0));

    // Количество единиц в gameGrid
    int onesCount = std::rand() % ((rows_ * cols_) / 5) + 1;

    // Создадим пустую gameGrid
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            gameGrid_[i][j] = 0;
        }
    }

    // Расставим единицы случайным образом
    for (int k = 0; k < onesCount; ++k) {
        int randomRow = std::rand() % rows_;
        int randomCol = std::rand() % cols_;

        // Проверим, что выбранная ячейка еще не содержит 1
        while (gameGrid_[randomRow][randomCol] == 1) {
        randomRow = std::rand() % rows_;
        randomCol = std::rand() % cols_;
        }

        gameGrid_[randomRow][randomCol] = 1;
    }

    // Расставим одну двойку
    int randomRow = std::rand() % rows_;
    int randomCol = std::rand() % cols_;

    gameGrid_[randomRow][randomCol] = 2;
    player_.setX(randomCol);
    player_.setY(randomRow);
    qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
}

void MainWindow::setupGameGrid() {
    // Установите размер ячеек

// Заполните ячейки таблицы значениями из игровой сетки
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            tableWidget_->setItem(i, j, item);

            switch (gameGrid_[i][j]) {
            case 0: // Empty space
                item->setBackground(QColor(Qt::white));
                break;
            case 1: // Wall
                item->setBackground(QColor(Qt::darkGray));
                break;
            case 2: // player
                item->setIcon(QIcon(":resource/puckman.png").pixmap(QSize(64, 64)));
                break;
            }
        }
    }

    QSize tablesize = tableWidget_->sizeHint();
    this->resize(tablesize);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
            movePlayerUp();
            break;
        case Qt::Key_S:
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
            movePlayerDown();
            break;
        case Qt::Key_A:
            movePlayerLeft();
            break;
        case Qt::Key_D:
            movePlayerRight();
            break;
        default:
            QMainWindow::keyPressEvent(event);
            break;
    }
}

void MainWindow::movePlayerUp() {
    if(gameGrid_[player_.getY()-1][player_.getX()] != 1)
    {
        if (player_.getY() > 0) {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            gameGrid_[player_.getY()-1][player_.getX()] = 2;
            player_.setY(player_.getY() - 1);
            setupGameGrid();
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerDown() {
    int rowCount = tableWidget_->rowCount();
    if(gameGrid_[player_.getY()+1][player_.getX()] != 1)
    {
        if (player_.getY()+1 < rowCount) {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            gameGrid_[player_.getY()-1][player_.getX()] = 2;
            player_.setY(player_.getY() - 1);
            setupGameGrid();
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerLeft() {
    if(gameGrid_[player_.getY()][player_.getX()-1] != 1)
    {
        if (player_.getX() > 0) {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            gameGrid_[player_.getY()][player_.getX()-1] = 2;
            player_.setX(player_.getX() - 1);
            setupGameGrid();
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}

void MainWindow::movePlayerRight() {
    int columnCount = tableWidget_->columnCount();
    if(gameGrid_[player_.getY()][player_.getX()+1] != 1)
    {
        if (player_.getX() < columnCount) {
            gameGrid_[player_.getY()][player_.getX()] = 0;
            gameGrid_[player_.getY()][player_.getX()+1] = 2;
            player_.setX(player_.getX() + 1);
            setupGameGrid();
            qDebug() << "Player Coordinates: (" << player_.getX() << ", " << player_.getY() << ")";
        }
    }
}
