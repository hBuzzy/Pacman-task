#include "startgame.h"
#include "ui_startgame.h"
#include "mainwindow.h"
#include "redactor.h"

StartGame::StartGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartGame)
{
    ui->setupUi(this);
    loadUI();
}

StartGame::~StartGame()
{
    delete ui;
}

void StartGame::loadUI(){
    ui->play_button->setFont(QFont(default_font_family_, font_size_));
    ui->redactor_button->setFont(QFont(default_font_family_, font_size_));
}

void StartGame::on_play_button_clicked()
{
    this->close();
    MainWindow *game = new MainWindow();
    game->show();
}

void StartGame::on_redactor_button_clicked()
{
    this->close();
    Redactor *game = new Redactor();
    game->show();
}
