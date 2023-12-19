#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "hostile.h"
#include "gameelement.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QTimer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int** gameGrid = nullptr);
    ~MainWindow();

    void generateRandomGameGrid();
    void setupGameGrid();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QTimer* gameTimer_;
    QTimer* hostileRunTimer_;
    float gameTime_;
    const int kRows_ = 10;
    const int kColumns_ = 10;
    int **gameGrid_;
    int cellSize_ = 64;
    int counsCount_ = 5;
    bool isKeyTime_ = true;
    Player player_;
    QGraphicsScene *scene_;
    QGraphicsView *view_;
    std::vector<Hostile> hostiles_;

    void movePlayer(Directions direction);
    void moveHostile();
    void generateRandomElements(int element, int count);
    void updateCoinsCount();
    void gameOver(bool isWin);
    void handleGameOver();

private slots:
    void updateGameTime();
    void updateHostileRunTime();

};

#endif // MAINWINDOW_H
