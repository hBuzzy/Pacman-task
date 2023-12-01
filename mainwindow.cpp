#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pacman_ = new PacmanArea();
    ui->sceneLayout->addWidget(pacman_);

    connect(ui->startBtn, &QPushButton::clicked, pacman_, &PacmanArea::startGame);
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::setFocuse);
    connect(ui->stopBtn, &QPushButton::clicked, pacman_, &PacmanArea::stopGame);
}

void MainWindow::setFocuse() {
    pacman_->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

