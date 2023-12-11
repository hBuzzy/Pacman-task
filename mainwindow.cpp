#include "mainwindow.h"

#include <qgraphicsitem.h>
#include <dragitem.h>

#include <QCoreApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);

  SetUi();
  SetConnections();

  ui_->graphicsView->setFocusPolicy(Qt::StrongFocus);
  //ui_->listWidget->setFocusPolicy(Qt::NoFocus);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::AddNewItem() {
  // Рисуем эллипс, вписанный в прямоугольник стандартным методом
  QGraphicsItem *figure = scene_->addEllipse(
      QRectF(0, 0, 70, 40),  // Геометрия фигуры
      QPen(Qt::black),       // Цвет контура фигуры
      QBrush(Qt::blue, Qt::SolidPattern));  // Цвет заливки фигуры

  // Включам возможность претаскивать объект по сцене
  figure->setFlag(QGraphicsItem::ItemIsMovable, true);
  // Включам возможность выделять объект на сцене
  figure->setFlag(QGraphicsItem::ItemIsSelectable, true);

  // Эквивалентная запись флагов, установленных выше
  figure->setFlags(QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsSelectable);
}

//void MainWindow::AddItemToList(ItemsFactory::ItemsType itemType) {
//  DragItem *item = new DragItem(itemType);
//  ui_->listWidget->addItem(item);
//  ui_->listWidget->setItemWidget(item, item);
//}

void MainWindow::AddNewCoinItem() {
  scene_->createEvent(ItemsFactory::ItemsType::kCoin);
  qDebug() << scene_->getWalls() << scene_->getCoins() << scene_->getPlayer();
  // scene_->addItem(ItemsFactory::Create(ItemsFactory::ItemsType::kCoin));
}

void MainWindow::AddNewWallItem() {
  scene_->createEvent(ItemsFactory::ItemsType::kWall);
  // scene_->addItem(ItemsFactory::Create(ItemsFactory::ItemsType::kWall));
}

void MainWindow::AddNewPlayerItem() {
  scene_->createEvent(ItemsFactory::ItemsType::kPlayer);
  // scene_->addItem(ItemsFactory::Create(ItemsFactory::ItemsType::kPlayer));
}

void MainWindow::UpdateScoreLSD(uint score) {
  //ui_->scoreLCD->display(score);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    auto selectedItems = scene_->selectedItems();
    // Если есть выделенные объекты на сцене, берем первый из них
    if (!selectedItems.isEmpty()) {
      auto item = selectedItems[0];
    }
  }
}

void MainWindow::SetWidgetMoveEnabled(bool enabled) {

}

void MainWindow::SetConnections() {

  connect(ui_->startButton, &QPushButton::clicked, game_,
            &PacmanGame::startNewGame);
  connect(ui_->startButton, &QPushButton::clicked,
            [=]() { scene_->setFiguresDragAndDropOption(false); });

  connect(ui_->newItemButon, &QPushButton::clicked, this,
          &MainWindow::AddNewWallItem);

  connect(ui_->newPlayerButton, &QPushButton::clicked, this,
          &MainWindow::AddNewPlayerItem);

  connect(ui_->newCoinItemButton, &QPushButton::clicked, this,
          &MainWindow::AddNewCoinItem);

  connect(ui_->rotateButton, &QPushButton::clicked, scene_,
          &Scene::RotateSelectedItems);

  // Увеличиваем масштаб объектов на itemScaleFactor_
  connect(ui_->zoomInButton, &QPushButton::clicked, this,
          [=]() { scene_->ScaleSelectedItems(itemScaleFactor_); });

  // Уменьшаем масштаб объектов на itemScaleFactor_
  connect(ui_->zoomOutButton, &QPushButton::clicked, this,
          [=]() { scene_->ScaleSelectedItems(-itemScaleFactor_); });

  // Сбрасываем масштаб выделенных объектов
  connect(ui_->resetZoomButton, &QPushButton::clicked, scene_,
          &Scene::ResetSelectedItemsScale);

  // Удаляем выделенные объекты по нажатию кнопки
  connect(ui_->removeButton, &QPushButton::clicked, scene_,
          &Scene::RemoveSelectedItems);
}

void MainWindow::SetUi() {
  scene_ = new Scene();
  game_ = new PacmanGame(*scene_, this);

  ui_->graphicsView->setScene(scene_);
  ui_->graphicsView->setRenderHint(QPainter::Antialiasing);

  show();

  QRectF viewRect = ui_->graphicsView->geometry();
  scene_->setSceneRect(viewRect);

//  AddNewOwnItem(ItemsFactory::ItemsType::kWall);
//  AddNewOwnItem(ItemsFactory::ItemsType::kPlayer);
//  AddNewOwnItem(ItemsFactory::ItemsType::kCoin);
}

void MainWindow::addItemToList(ItemsFactory::ItemsType itemType) {
  DragItem *item = new DragItem(itemType);

}

