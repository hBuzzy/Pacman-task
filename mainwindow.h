#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "hostile.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void generateRandomGameGrid();
    void setupGameGrid(); // Объявление функции setupGameGrid

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QTimer* gameTimer_;
    float gameTime_;
    const int rows_ = 10;
    const int cols_ = 10;
    int **gameGrid_;
    int gridSize_ = 64;
    bool isKeyTime_ = true;
    Player player_;
    std::vector<Hostile> hostiles_;
    void movePlayerUp();
    void movePlayerDown();
    void movePlayerLeft();
    void movePlayerRight();
    void moveHostile();
    QGraphicsScene *scene;
    QGraphicsView *view;
    void generateRandomElements(int element, int count);

private slots:
    void updateGameTime();
    void handleKeyRepeat();

};

#endif // MAINWINDOW_H
