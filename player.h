#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player();

    int getX() const;
    int getY() const;
    void setX(int newX);
    void setY(int newY);

private:
    int x_;
    int y_;
};

#endif // PLAYER_H
