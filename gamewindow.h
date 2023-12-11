#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "pacmanarea/pacman/pacmanitem.h"

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(QVector<QVector<int>> boxArea, QWidget *parent = nullptr);
protected:
    void updateGame();
    void paintEvent(QPaintEvent* event);

signals:

private:
    QVector<QVector<int>> boxArea_;
    PacManItem* pacman_;
};

#endif // GAMEWINDOW_H
