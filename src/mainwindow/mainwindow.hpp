#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <ranges>

#include "ui_mainwindow.h"
#include "scene/scene.hpp"
#include "scene/objects/wall.hpp"
#include "scene/objects/coin.hpp"
#include "scene/objects/pacman.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

template<typename First, typename... Rest>
void addListItems(Ui::MainWindow* ui) {
    auto* item = First::getFigure();
    SceneObject::factories.emplace(item->GetSceneObject()->GetType(), &First::getInstance);
    ui->figuresList->addItem(item);
    ui->figuresList->setItemWidget(item, item);
    if constexpr(sizeof...(Rest))
        addListItems<Rest...>(ui);
}

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr)
    : QMainWindow(parent), ui(new Ui::MainWindow), scene_(this) {
        ui->setupUi(this);
        ui->viewport->setScene(&scene_);
        ui->viewport->setRenderHints(QPainter::Antialiasing);
        addListItems<Wall, Coin, Pacman>(ui);
        connect(ui->deleteButton, &QPushButton::clicked, &scene_, &Scene::RemoveSelected);
        connect(ui->topPlus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({1, 0, 0, 0});
        });
        connect(ui->topMinus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({-1, 0, 0, 0});
        });
        connect(ui->leftPlus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({0, 1, 0, 0});
        });
        connect(ui->leftMinus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({0, -1, 0, 0});
        });
        connect(ui->bottomPlus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({0, 0, 1, 0});
        });
        connect(ui->bottomMinus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({0, 0, -1, 0});
        });
        connect(ui->rightPlus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({0, 0, 0, 1});
        });
        connect(ui->rightMinus, &QPushButton::clicked, [this](){
            scene_.AdjustWallsStretching({0, 0, 0, -1});
        });
        connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::StartGame);
        connect(ui->stopButton, &QPushButton::clicked, &scene_, &Scene::GameStopped);
        connect(&scene_, &Scene::GameStopped, this, &MainWindow::StopGame);
        connect(&scene_, &Scene::ScoreUpdated, ui->score,
            static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));
        connect(ui->about, &QAction::triggered, this, &MainWindow::ShowHelp);
    }
    ~MainWindow() final { delete ui; };
private slots:
    void StartGame() {
        if (scene_.StartGame()) {
            ui->startButton->setEnabled(false);
            ui->stopButton->setEnabled(true);
            ui->topPlus->setEnabled(false);
            ui->topMinus->setEnabled(false);
            ui->leftPlus->setEnabled(false);
            ui->leftMinus->setEnabled(false);
            ui->bottomPlus->setEnabled(false);
            ui->bottomMinus->setEnabled(false);
            ui->rightPlus->setEnabled(false);
            ui->rightMinus->setEnabled(false);
            ui->deleteButton->setEnabled(false);
            ui->figuresList->setEnabled(false);
        }
    }
    void StopGame() {
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->topPlus->setEnabled(true);
        ui->topMinus->setEnabled(true);
        ui->leftPlus->setEnabled(true);
        ui->leftMinus->setEnabled(true);
        ui->bottomPlus->setEnabled(true);
        ui->bottomMinus->setEnabled(true);
        ui->rightPlus->setEnabled(true);
        ui->rightMinus->setEnabled(true);
        ui->deleteButton->setEnabled(true);
        ui->figuresList->setEnabled(true);
    }
    void ShowHelp() const {
        QMessageBox::about((QWidget*)this, "How to play",
            "Controls:\n"
            "\t-Go Up: Up Key;\n"
            "\t-Go Left: Left Key;\n"
            "\t-Go Down: Down Key;\n"
            "\t-Go Right: Right Key;\n"
            "\nGame rules:\n"
            "\t- setup the scene in editor before you start the game;\n"
            "\t- collect all coins you have placed to win;\n"
        );
    }
private:
    Ui::MainWindow* ui;
    Scene scene_;
};

#endif
