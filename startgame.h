#ifndef STARTGAME_H
#define STARTGAME_H

#include <QWidget>

namespace Ui {
class StartGame;
}

class StartGame : public QWidget
{
    Q_OBJECT

public:
    explicit StartGame(QWidget *parent = nullptr);
    ~StartGame();

private slots:
    void on_play_button_clicked();
    void on_redactor_button_clicked();

private:
    Ui::StartGame *ui;
    static const int font_size_ = 15;
    const QString default_font_family_ = "Default";
    void loadUI();
};

#endif // STARTGAME_H
