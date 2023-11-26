#include "mainwindow.h"

#include <QCoreApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QMessageBox>

#include "dragitem.h"
#include "qgraphicsitem.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  SetUi();

  SetConnections();
  ui_->graphicsView->setFocusPolicy(Qt::StrongFocus);
  ui_->listWidget->setFocusPolicy(Qt::NoFocus);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::SetListWidgetEnabled(bool enabled) {
  ui_->listWidget->setEnabled(enabled);
}

void MainWindow::UpdateScoreLabel(int score) {
  ui_->counterLCD->display(score);
}

void MainWindow::SetConnections() {
  menuBar()->setStyleSheet(QString("font-size: %1px").arg(14));

  QMenu *gameMenu = menuBar()->addMenu("&Меню");
  QAction *help = new QAction("&Справка", this);

  gameMenu->addAction(help);
  connect(help, &QAction::triggered, this, &MainWindow::ShowHelp);

  connect(ui_->newGameButton, &QPushButton::clicked, game_,
          &Game::StartNewGame);
  connect(ui_->newGameButton, &QPushButton::clicked,
          [=]() { scene_->SetFiguresInteraction(false); });

  connect(ui_->stopGameButton, &QPushButton::clicked, game_, &Game::StopGame);
  connect(ui_->stopGameButton, &QPushButton::clicked,
          [=]() { ui_->listWidget->setEnabled(true); });
  connect(ui_->stopGameButton, &QPushButton::clicked,
          [=]() { scene_->SetFiguresInteraction(true); });

  connect(game_, &Game::ScoreChanged, this, &MainWindow::UpdateScoreLabel);
}

void MainWindow::ShowHelp() {
  QMessageBox::about(
      this, "Управление",
      "<br><b>Клавиша →</b>  Двигаться вправо \n</br>"
      "<br><b>Клавиша ←</b>  Двигаться влево \n</br>"
      "<br><b>Клавиша ↑</b>  Двигаться вверх \n</br>"
      "<br><b>Клавиша ↓</b>  Двигаться вниз \n</br>"
      "<br><b>Клавиша del </b> Удалить выбранные объекты \n</br>"
      "<br><b>Клавиша R ↓</b> Повернуть выбранные объекты по ч.с.\n</br>");
}

void MainWindow::AddItemToList(ItemsFactory::ItemsType itemType) {
  DragItem *item = new DragItem(itemType);
  ui_->listWidget->addItem(item);
  ui_->listWidget->setItemWidget(item, item);
}

void MainWindow::SetUi() {
  scene_ = new Scene();
  game_ = new Game(*scene_, this);

  ui_->graphicsView->setScene(scene_);
  ui_->graphicsView->setRenderHint(QPainter::Antialiasing);

  show();

  ui_->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui_->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QRectF viewRect = ui_->graphicsView->geometry();
  scene_->setSceneRect(viewRect);

  AddItemToList(ItemsFactory::ItemsType::kWall);
  AddItemToList(ItemsFactory::ItemsType::kPlayer);
  AddItemToList(ItemsFactory::ItemsType::kCoin);
}
