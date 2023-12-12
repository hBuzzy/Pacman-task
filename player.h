// player.h
#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    int x_;  // Координата x
    int y_;  // Координата y

public:

    Player();

    int getX() const;
    int getY() const;
    void setX(int newX);
    void setY(int newY);
};

#endif // PLAYER_H
