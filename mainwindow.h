#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"

#include <QTableWidgetItem>
#include <QMainWindow>

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
    QTableWidget* tableWidget_;
    const int rows_ = 5;
    const int cols_ = 5;
    int **gameGrid_;
    Player player_;
    void movePlayerUp();
    void movePlayerDown();
    void movePlayerLeft();
    void movePlayerRight();

};

#endif // MAINWINDOW_H
