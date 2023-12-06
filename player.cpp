// player.cpp
#include "player.h"

// Реализация конструктора по умолчанию
Player::Player() : x_(0), y_(0) {}

// Геттеры и сеттеры
int Player::getX() const {
    return x_;
}

int Player::getY() const {
    return y_;
}

void Player::setX(int newX) {
    x_ = newX;
}

void Player::setY(int newY) {
    y_ = newY;
}
