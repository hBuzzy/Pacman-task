#include "mainwindow.h"

#include <qgraphicsitem.h>

#include <QDebug>
#include <QGraphicsItem>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);

  SetUi();
  SetConnections();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::SetListWidgetEnabled(bool enabled) {
  ui_->listWidget->setEnabled(enabled);
}

void MainWindow::UpdateScoreLabel(int score) {
  ui_->counterLCD->display(score);
}

void MainWindow::SetConnections() {
  connect(ui_->rotateButton, &QPushButton::clicked, scene_,
          &Scene::RotateSelectedItems);
  connect(ui_->zoomInButton, &QPushButton::clicked, this,
          [=]() { scene_->ScaleSelectedItems(itemScaleFactor_); });
  connect(ui_->zoomOutButton, &QPushButton::clicked, this,
          [=]() { scene_->ScaleSelectedItems(-itemScaleFactor_); });
  connect(ui_->resetZoomButton, &QPushButton::clicked, scene_,
          &Scene::ResetSelectedItemsScale);
  connect(ui_->removeButton, &QPushButton::clicked, scene_,
          &Scene::RemoveSelectedItems);
  connect(ui_->startButton, &QPushButton::clicked, game_, &Game::StartGame);
  connect(ui_->graphicsViewRotateButton, &QPushButton::clicked, this,
          [=]() { ui_->graphicsView->rotate(rotationAngle_); });
  connect(ui_->startButton, &QPushButton::clicked,
          [=]() { scene_->SetFiguresInteraction(false); });
  connect(ui_->pauseButton, &QPushButton::clicked, game_, &Game::StopGame);
  connect(ui_->pauseButton, &QPushButton::clicked,
          [=]() { ui_->listWidget->setEnabled(true); });
  connect(ui_->pauseButton, &QPushButton::clicked,
          [=]() { scene_->SetFiguresInteraction(true); });
  connect(game_, &Game::ScoreChanged, this, &MainWindow::UpdateScoreLabel);
  connect(ui_->pauseButton, &QPushButton::clicked, game_, &Game::StopGame);
  connect(ui_->aboutButton, &QPushButton::clicked, game_, &Game::ShowAbout);
}

void MainWindow::SetUi() {
  scene_ = new Scene(this);
  game_ = new Game(*scene_, this);
  ui_->graphicsView->setScene(scene_);
  AddItemToList(ItemsFactory::kBorder);
  AddItemToList(ItemsFactory::kCoin);
  AddItemToList(ItemsFactory::kPlayer);
}

void MainWindow::AddItemToList(const QString &itemType) {
  DragItem *item = new DragItem(itemType);
  ui_->listWidget->addItem(item);
  ui_->listWidget->setItemWidget(item, item);
}
